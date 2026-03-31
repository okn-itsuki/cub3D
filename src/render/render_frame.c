#include "ray_casting.h"
#include "mlx.h"

static void	fill_row(t_img *frame, int y, uint32_t color)
{
	uint32_t	*dst;
	int			x;

	dst = (uint32_t *)(frame->addr + y * frame->line_len);
	x = 0;
	while (x < frame->width)
	{
		dst[x] = color;
		++x;
	}
}

static void	fill_background(t_img *frame, uint32_t ceil_color,
		uint32_t floor_color)
{
	int	y;

	y = 0;
	while (y < frame->height / 2)
	{
		fill_row(frame, y, ceil_color);
		++y;
	}
	while (y < frame->height)
	{
		fill_row(frame, y, floor_color);
		++y;
	}
}

static void	render_columns(t_game *game)
{
	int	col;

	col = 0;
	while (col < WIN_W)
	{
		cast_ray(&game->render.ray, &game->player, &game->config.map, col);
		render_column(&game->render, &game->assets);
		++col;
	}
}

void	render_frame(t_game *game)
{
	fill_background(&game->render.frame, game->config.ceiling_color.value,
		game->config.floor_color.value);
	render_columns(game);
	mlx_put_image_to_window(game->mlx.mlx, game->mlx.win,
		game->render.frame.img, 0, 0);
}
