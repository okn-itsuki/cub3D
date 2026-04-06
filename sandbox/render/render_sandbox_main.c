#include "cub3d.h"
#include "game_init.h"
#include "ray_casting.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG_DASHBOARD
# include "debug/debug_dashboard.h"
#endif

/*
** parser を通さずに描画と raycast を試すための簡易エントリポイント。
** maps/good/library.cub 相当の t_config を手動で組み立てる。
*/

static bool	init_tex_path(t_tex_path *tex)
{
	tex->path[TEX_NO] = strdup("../../texture/direction/north.xpm");
	tex->path[TEX_SO] = strdup("../../texture/direction/south.xpm");
	tex->path[TEX_WE] = strdup("../../texture/direction/west.xpm");
	tex->path[TEX_EA] = strdup("../../texture/direction/east.xpm");
	return (tex->path[TEX_NO] != NULL && tex->path[TEX_SO] != NULL
		&& tex->path[TEX_WE] != NULL && tex->path[TEX_EA] != NULL);
}

static void	update_color_value(t_rgb *color)
{
	color->value = (color->r << 16) | (color->g << 8) | color->b;
}

static void	init_colors(t_rgb *floor_color, t_rgb *ceiling_color)
{
	floor_color->r = 255;
	floor_color->g = 255;
	floor_color->b = 255;
	floor_color->is_set = true;
	update_color_value(floor_color);
	ceiling_color->r = 0;
	ceiling_color->g = 0;
	ceiling_color->b = 0;
	ceiling_color->is_set = true;
	update_color_value(ceiling_color);
}

static const char	*g_map_src[] = {
	"1111111111111111111",
	"1001001001001001001",
	"1001001001001001001",
	"1001001001001001001",
	"1001001000001001001",
	"100000000S000000001",
	"1000011111111000001",
	"1111000000000001111",
	"1000000101010000001",
	"1111111111111111111"
};

static int	get_map_height(void)
{
	return ((int)(sizeof(g_map_src) / sizeof(g_map_src[0])));
}

static int	get_map_width(void)
{
	int		i;
	int		width;
	size_t	row_len;

	i = 0;
	width = 0;
	while (i < get_map_height())
	{
		row_len = strlen(g_map_src[i]);
		if ((int)row_len > width)
			width = (int)row_len;
		++i;
	}
	return (width);
}

static bool	get_spawn_dir(char cell, t_dir *dir)
{
	if (cell == 'N')
		*dir = NORTH;
	else if (cell == 'S')
		*dir = SOUTH;
	else if (cell == 'E')
		*dir = EAST;
	else if (cell == 'W')
		*dir = WEST;
	else
		return (false);
	return (true);
}

static bool	init_map_and_spawn(t_map *map, t_spawn *spawn)
{
	t_dir	dir;
	int		x;
	int		y;
	bool	found_spawn;

	map->height = get_map_height();
	map->width = get_map_width();
	map->grid = calloc((size_t)map->height + 1, sizeof(*map->grid));
	if (map->grid == NULL)
		return (false);
	found_spawn = false;
	y = 0;
	while (y < map->height)
	{
		map->grid[y] = strdup(g_map_src[y]);
		if (map->grid[y] == NULL)
			return (false);
		x = 0;
		while (map->grid[y][x] != '\0')
		{
			if (get_spawn_dir(map->grid[y][x], &dir))
			{
				if (found_spawn)
					return (false);
				found_spawn = true;
				spawn->row = y;
				spawn->col = x;
				spawn->dir = dir;
				map->grid[y][x] = '0';
			}
			++x;
		}
		++y;
	}
	return (found_spawn);
}

static bool	init_sandbox_config(t_config *config)
{
	init_config(config);
	if (!init_tex_path(&config->tex))
	{
		destroy_config(config);
		return (false);
	}
	init_colors(&config->floor_color, &config->ceiling_color);
	if (!init_map_and_spawn(&config->map, &config->spawn))
	{
		destroy_config(config);
		return (false);
	}
	return (true);
}

void	debug_print_texture_image_all(const t_assets *assets);
void	debug_print_player_pos_all(const t_player player);

#ifdef DEBUG_DASHBOARD
typedef struct s_sandbox_ctx
{
	t_game		*game;
	t_dashboard	*dash;
}	t_sandbox_ctx;
#endif

static void	shutdown_sandbox(t_game *game, int exit_code)
{
#ifdef DEBUG_DASHBOARD
	dashboard_cleanup();
#endif
	destroy_game(game);
	exit(exit_code);
}

#ifdef DEBUG_DASHBOARD
static int	sandbox_loop_hook(t_sandbox_ctx *ctx)
{
	t_game_tick_status	status;

	status = game_loop_tick(ctx->game);
	if (status == GAME_TICK_ERROR)
		shutdown_sandbox(ctx->game, EXIT_FAILURE);
	if (status == GAME_TICK_STOP)
		shutdown_sandbox(ctx->game, EXIT_SUCCESS);
	dashboard_update(ctx->dash, ctx->game);
	return (0);
}
#else

static int	sandbox_loop_hook(t_game *game)
{
	t_game_tick_status	status;

	status = game_loop_tick(game);
	if (status == GAME_TICK_ERROR)
		shutdown_sandbox(game, EXIT_FAILURE);
	if (status == GAME_TICK_STOP)
		shutdown_sandbox(game, EXIT_SUCCESS);
	return (0);
}
#endif

int	main(void)
{
	t_game	game;
	bool	ok;

#ifdef DEBUG_DASHBOARD
	t_dashboard		dash;
	t_sandbox_ctx	ctx;
#endif

	init_game(&game);
	if (!init_sandbox_config(&game.config))
		return (1);
	ok = init_game_mlx(&game);
	if (ok)
		ok = init_game_wall_textures(&game, game.config.tex);
	if (ok)
		init_player(&game.player, game.config.spawn);
	if (!ok)
	{
		destroy_game(&game);
		return (1);
	}
	debug_print_texture_image_all(&game.assets);
	debug_print_player_pos_all(game.player);
	game.running = true;
	register_hooks(&game);
#ifdef DEBUG_DASHBOARD
	dashboard_init(&dash);
	ctx.game = &game;
	ctx.dash = &dash;
	mlx_loop_hook(game.mlx.mlx, sandbox_loop_hook, &ctx);
#else
	mlx_loop_hook(game.mlx.mlx, sandbox_loop_hook, &game);
#endif
	mlx_loop(game.mlx.mlx);
#ifdef DEBUG_DASHBOARD
	dashboard_cleanup();
#endif
	destroy_game(&game);
	return (0);
}
