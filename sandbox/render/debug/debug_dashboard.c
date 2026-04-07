#ifdef DEBUG_DASHBOARD

# include "debug_dashboard.h"
# ifdef BONUS_MODE
#  include "game_config_bonus.h"
# else
#  include "game_config.h"
# endif

# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/ioctl.h>
# include <sys/time.h>

# define DASH_BUF_SIZE 16384
# define DASH_FD STDERR_FILENO
# define PANEL_W 46

/* ─── ANSI escape sequences ─── */

# define ESC          "\033["
# define CUR_HIDE     ESC "?25l"
# define CUR_SHOW     ESC "?25h"
# define RST          ESC "0m"
# define BOLD         ESC "1m"
# define DIM          ESC "2m"
# define FG_RED       ESC "31m"
# define FG_GREEN     ESC "32m"
# define FG_YELLOW    ESC "33m"
# define FG_CYAN      ESC "36m"
# define FG_WHITE     ESC "37m"
# define FG_MAGENTA   ESC "35m"

/* ─── time util ─── */

static uint64_t	now_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((uint64_t)tv.tv_sec * 1000000 + (uint64_t)tv.tv_usec);
}

/* ─── terminal size ─── */

static void	get_term_size(int *rows, int *cols)
{
	struct winsize	ws;

	if (ioctl(DASH_FD, TIOCGWINSZ, &ws) == 0)
	{
		*rows = ws.ws_row;
		*cols = ws.ws_col;
	}
	else
	{
		*rows = 24;
		*cols = 80;
	}
}

/* ─── buffer write helpers ─── */

static int	buf_add(char *buf, int pos, int max, const char *s)
{
	int	i;

	i = 0;
	while (s[i] && pos < max - 1)
	{
		buf[pos] = s[i];
		pos++;
		i++;
	}
	return (pos);
}

static int	buf_goto(char *buf, int pos, int max, int row, int col)
{
	char	tmp[32];

	snprintf(tmp, sizeof(tmp), "\033[%d;%dH", row, col);
	return (buf_add(buf, pos, max, tmp));
}

/* ─── box drawing ─── */

static int	draw_hline(char *buf, int pos, int max, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pos = buf_add(buf, pos, max, "─");
		i++;
	}
	return (pos);
}

static int	draw_border(char *buf, int pos, int max,
	int row, int col, const char *l, const char *r_ch)
{
	pos = buf_goto(buf, pos, max, row, col);
	pos = buf_add(buf, pos, max, FG_CYAN);
	pos = buf_add(buf, pos, max, l);
	pos = draw_hline(buf, pos, max, PANEL_W - 2);
	pos = buf_add(buf, pos, max, r_ch);
	pos = buf_add(buf, pos, max, RST);
	return (pos);
}

/*
** write one content row: left border + content via callback-style
** we use a macro-like approach: call row_open, then add content, then row_close
*/

static int	row_open(char *buf, int pos, int max, int row, int col)
{
	pos = buf_goto(buf, pos, max, row, col);
	pos = buf_add(buf, pos, max, FG_CYAN "│" RST " ");
	return (pos);
}

static int	row_close(char *buf, int pos, int max, int row, int col)
{
	pos = buf_goto(buf, pos, max, row, col + PANEL_W - 1);
	pos = buf_add(buf, pos, max, FG_CYAN "│" RST);
	return (pos);
}

/* emit one complete content row (open + text + close) */
static int	emit_row(char *buf, int pos, int max, int *row,
	int col, const char *text)
{
	pos = row_open(buf, pos, max, *row, col);
	pos = buf_add(buf, pos, max, text);
	pos = row_close(buf, pos, max, *row, col);
	(*row)++;
	return (pos);
}

/* ─── FPS update ─── */

static void	update_fps(t_fps_stats *fps, uint64_t *prev, uint64_t cur)
{
	double	delta;

	if (*prev == 0)
	{
		*prev = cur;
		fps->current = 0.0;
		fps->min = 1e9;
		fps->max = 0.0;
		fps->sum = 0.0;
		fps->count = 0;
		fps->average = 0.0;
		return ;
	}
	delta = (double)(cur - *prev) / 1000000.0;
	*prev = cur;
	if (delta > 0.0)
		fps->current = 1.0 / delta;
	else
		fps->current = 0.0;
	if (fps->current < fps->min)
		fps->min = fps->current;
	if (fps->current > fps->max)
		fps->max = fps->current;
	fps->sum += fps->current;
	fps->count++;
	if (fps->count > 0)
		fps->average = fps->sum / fps->count;
}

static const char	*fps_color(double fps)
{
	if (fps >= 30.0)
		return (FG_GREEN);
	if (fps >= 15.0)
		return (FG_YELLOW);
	return (FG_RED);
}

/* ─── helper string converters ─── */

static const char	*dir_to_str(t_dir d)
{
	if (d == NORTH)
		return ("NORTH");
	if (d == SOUTH)
		return ("SOUTH");
	if (d == EAST)
		return ("EAST");
	if (d == WEST)
		return ("WEST");
	return ("???");
}

static const char	*hit_side_str(t_hit_side s)
{
	if (s == HIT_X)
		return ("HIT_X");
	return ("HIT_Y");
}

static const char	*tex_id_str(t_tex_id id)
{
	if (id == TEX_NO)
		return ("NO");
	if (id == TEX_SO)
		return ("SO");
	if (id == TEX_WE)
		return ("WE");
	if (id == TEX_EA)
		return ("EA");
	return ("??");
}

/* ─── section renderers ─── */
/* each takes int *row and advances it, returns updated buf pos */

static int	sect_fps(char *buf, int pos, int max,
	int *row, int col, const t_fps_stats *fps)
{
	char	tmp[128];

	pos = emit_row(buf, pos, max, row, col,
			BOLD FG_YELLOW " FPS" RST);
	snprintf(tmp, sizeof(tmp), " Current: %s%.1f" RST,
		fps_color(fps->current), fps->current);
	pos = emit_row(buf, pos, max, row, col, tmp);
	snprintf(tmp, sizeof(tmp), " Average: " FG_CYAN "%.1f" RST,
		fps->average);
	pos = emit_row(buf, pos, max, row, col, tmp);
	snprintf(tmp, sizeof(tmp), " Min/Max: " DIM "%.1f" RST
		" / " BOLD "%.1f" RST, fps->min, fps->max);
	pos = emit_row(buf, pos, max, row, col, tmp);
	return (pos);
}

static int	sect_player(char *buf, int pos, int max,
	int *row, int col, const t_player *p, const t_spawn *sp)
{
	char	tmp[128];

	pos = emit_row(buf, pos, max, row, col,
			BOLD FG_YELLOW " PLAYER" RST);
	snprintf(tmp, sizeof(tmp), " pos   : " FG_CYAN "(%.3f, %.3f)" RST,
		p->pos.x, p->pos.y);
	pos = emit_row(buf, pos, max, row, col, tmp);
	snprintf(tmp, sizeof(tmp), " dir   : " FG_CYAN "(%.3f, %.3f)" RST,
		p->dir.x, p->dir.y);
	pos = emit_row(buf, pos, max, row, col, tmp);
	snprintf(tmp, sizeof(tmp), " plane : " FG_CYAN "(%.3f, %.3f)" RST,
		p->plane.x, p->plane.y);
	pos = emit_row(buf, pos, max, row, col, tmp);
	snprintf(tmp, sizeof(tmp), " speed : " FG_WHITE "mv=%.2f rot=%.2f" RST,
		p->move_speed, p->rot_speed);
	pos = emit_row(buf, pos, max, row, col, tmp);
	snprintf(tmp, sizeof(tmp), " spawn : " DIM "(%d,%d) %s" RST,
		sp->col, sp->row, dir_to_str(sp->dir));
	pos = emit_row(buf, pos, max, row, col, tmp);
	return (pos);
}

static int	sect_ray(char *buf, int pos, int max,
	int *row, int col, const t_ray *r)
{
	char	tmp[128];

	pos = emit_row(buf, pos, max, row, col,
			BOLD FG_YELLOW " RAY (last column)" RST);
	snprintf(tmp, sizeof(tmp), " column    : " FG_WHITE "%d / %d" RST,
		r->column, WIN_W);
	pos = emit_row(buf, pos, max, row, col, tmp);
	snprintf(tmp, sizeof(tmp), " ray_dir   : " FG_CYAN "(%.4f, %.4f)" RST,
		r->ray_dir.x, r->ray_dir.y);
	pos = emit_row(buf, pos, max, row, col, tmp);
	snprintf(tmp, sizeof(tmp), " map_cell  : " FG_WHITE "(%d, %d)" RST,
		r->map.x, r->map.y);
	pos = emit_row(buf, pos, max, row, col, tmp);
	snprintf(tmp, sizeof(tmp), " perp_dist : " FG_GREEN "%.4f" RST,
		r->perp_wall_dist);
	pos = emit_row(buf, pos, max, row, col, tmp);
	snprintf(tmp, sizeof(tmp), " wall_x    : " FG_WHITE "%.4f" RST,
		r->wall_x);
	pos = emit_row(buf, pos, max, row, col, tmp);
	snprintf(tmp, sizeof(tmp), " hit_side  : " FG_MAGENTA "%s" RST,
		hit_side_str(r->hit_side));
	pos = emit_row(buf, pos, max, row, col, tmp);
	return (pos);
}

static int	sect_column(char *buf, int pos, int max,
	int *row, int col, const t_column *c)
{
	char	tmp[128];

	pos = emit_row(buf, pos, max, row, col,
			BOLD FG_YELLOW " COLUMN" RST);
	snprintf(tmp, sizeof(tmp), " draw : " FG_CYAN "%d..%d" RST
		" (h=%d)", c->draw_start, c->draw_end,
		c->draw_end - c->draw_start);
	pos = emit_row(buf, pos, max, row, col, tmp);
	snprintf(tmp, sizeof(tmp), " tex  : " FG_WHITE "%s" RST
		"  tex_x=%d", tex_id_str(c->tex_id), c->tex_x);
	pos = emit_row(buf, pos, max, row, col, tmp);
	snprintf(tmp, sizeof(tmp), " step : " DIM "%.4f" RST
		"  pos=%.2f", c->tex_step, c->tex_pos);
	pos = emit_row(buf, pos, max, row, col, tmp);
	return (pos);
}

static int	sect_clock(char *buf, int pos, int max,
	int *row, int col, const t_frame_clock *clk)
{
	char	tmp[128];

	pos = emit_row(buf, pos, max, row, col,
			BOLD FG_YELLOW " CLOCK" RST);
	snprintf(tmp, sizeof(tmp), " frame : " FG_CYAN "%llu" RST,
		(unsigned long long)clk->frame_index);
	pos = emit_row(buf, pos, max, row, col, tmp);
	snprintf(tmp, sizeof(tmp), " delta : " FG_WHITE "%.6f s" RST,
		clk->delta_sec);
	pos = emit_row(buf, pos, max, row, col, tmp);
	return (pos);
}

static int	sect_input(char *buf, int pos, int max,
	int *row, int col, const t_input *inp)
{
	char	tmp[128];

	pos = emit_row(buf, pos, max, row, col,
			BOLD FG_YELLOW " INPUT" RST);
	snprintf(tmp, sizeof(tmp),
		" [%c]fwd [%c]bck [%c]L [%c]R [%c]tL [%c]tR",
		inp->move_forward ? 'X' : ' ',
		inp->move_backward ? 'X' : ' ',
		inp->strafe_left ? 'X' : ' ',
		inp->strafe_right ? 'X' : ' ',
		inp->turn_left ? 'X' : ' ',
		inp->turn_right ? 'X' : ' ');
	pos = emit_row(buf, pos, max, row, col, tmp);
	return (pos);
}

static int	sect_colors(char *buf, int pos, int max,
	int *row, int col, const t_rgb *floor, const t_rgb *ceil)
{
	char	tmp[128];

	pos = emit_row(buf, pos, max, row, col,
			BOLD FG_YELLOW " COLORS" RST);
	snprintf(tmp, sizeof(tmp), " floor   : " FG_WHITE "(%d,%d,%d)" RST
		DIM " 0x%06X" RST, floor->r, floor->g, floor->b, floor->value);
	pos = emit_row(buf, pos, max, row, col, tmp);
	snprintf(tmp, sizeof(tmp), " ceiling : " FG_WHITE "(%d,%d,%d)" RST
		DIM " 0x%06X" RST, ceil->r, ceil->g, ceil->b, ceil->value);
	pos = emit_row(buf, pos, max, row, col, tmp);
	return (pos);
}

static int	sect_map(char *buf, int pos, int max,
	int *row, int col, const t_map *map, const t_player *p)
{
	char	tmp[128];
	int		my;
	int		mx;
	int		py;
	int		px;

	snprintf(tmp, sizeof(tmp), BOLD FG_YELLOW " MAP" RST
		DIM " (%dx%d)" RST, map->width, map->height);
	pos = emit_row(buf, pos, max, row, col, tmp);
	py = (int)p->pos.y;
	px = (int)p->pos.x;
	my = 0;
	while (my < map->height)
	{
		pos = row_open(buf, pos, max, *row, col);
		pos = buf_add(buf, pos, max, " ");
		mx = 0;
		while (mx < map->width && map->grid[my][mx])
		{
			if (my == py && mx == px)
				pos = buf_add(buf, pos, max, FG_GREEN BOLD "P" RST);
			else if (map->grid[my][mx] == '1')
				pos = buf_add(buf, pos, max, FG_WHITE "█" RST);
			else
				pos = buf_add(buf, pos, max, DIM "·" RST);
			mx++;
		}
		pos = row_close(buf, pos, max, *row, col);
		(*row)++;
		my++;
	}
	return (pos);
}

/* ─── public API ─── */

static void	push_screen(int fd, int rows)
{
	int	i;

	i = 0;
	while (i < rows)
	{
		write(fd, "\n", 1);
		i++;
	}
	write(fd, "\033[H", 3);
}

void	dashboard_init(t_dashboard *dash)
{
	memset(dash, 0, sizeof(*dash));
	get_term_size(&dash->term_rows, &dash->term_cols);
	dash->fps.min = 1e9;
	dash->prev_time_us = 0;
	dash->enabled = 1;
	push_screen(DASH_FD, dash->term_rows);
	write(DASH_FD, "\033[2J", 4);
	write(DASH_FD, CUR_HIDE, sizeof(CUR_HIDE) - 1);
}

void	dashboard_update(t_dashboard *dash, const t_game *game)
{
	char	buf[DASH_BUF_SIZE];
	int		p;
	int		r;
	int		c;

	if (!dash->enabled)
		return ;
	get_term_size(&dash->term_rows, &dash->term_cols);
	update_fps(&dash->fps, &dash->prev_time_us, now_us());
	p = 0;
	c = 1;
	r = 1;
	p = draw_border(buf, p, DASH_BUF_SIZE, r, c, "┌", "┐");
	r++;
	p = emit_row(buf, p, DASH_BUF_SIZE, &r, c,
			BOLD FG_GREEN " DEBUG DASHBOARD" RST);
	p = draw_border(buf, p, DASH_BUF_SIZE, r, c, "├", "┤");
	r++;
	p = sect_fps(buf, p, DASH_BUF_SIZE, &r, c, &dash->fps);
	p = draw_border(buf, p, DASH_BUF_SIZE, r, c, "├", "┤");
	r++;
	p = sect_player(buf, p, DASH_BUF_SIZE, &r, c, &game->player,
			&game->config.spawn);
	p = draw_border(buf, p, DASH_BUF_SIZE, r, c, "├", "┤");
	r++;
	p = sect_ray(buf, p, DASH_BUF_SIZE, &r, c, &game->render.ray);
	p = draw_border(buf, p, DASH_BUF_SIZE, r, c, "├", "┤");
	r++;
	p = sect_column(buf, p, DASH_BUF_SIZE, &r, c, &game->render.column);
	p = draw_border(buf, p, DASH_BUF_SIZE, r, c, "├", "┤");
	r++;
	p = sect_clock(buf, p, DASH_BUF_SIZE, &r, c, &game->clock);
	p = draw_border(buf, p, DASH_BUF_SIZE, r, c, "├", "┤");
	r++;
	p = sect_input(buf, p, DASH_BUF_SIZE, &r, c, &game->input);
	p = draw_border(buf, p, DASH_BUF_SIZE, r, c, "├", "┤");
	r++;
	p = sect_colors(buf, p, DASH_BUF_SIZE, &r, c,
			&game->config.floor_color, &game->config.ceiling_color);
	p = draw_border(buf, p, DASH_BUF_SIZE, r, c, "├", "┤");
	r++;
	p = sect_map(buf, p, DASH_BUF_SIZE, &r, c,
			&game->config.map, &game->player);
	p = draw_border(buf, p, DASH_BUF_SIZE, r, c, "└", "┘");
	write(DASH_FD, buf, (size_t)p);
}

void	dashboard_cleanup(void)
{
	write(DASH_FD, CUR_SHOW, sizeof(CUR_SHOW) - 1);
}

#endif
