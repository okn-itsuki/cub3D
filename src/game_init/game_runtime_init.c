#include "game_init.h"
#include "libft.h"

static void	reset_runtime_resources(t_game *game)
{
	ft_bzero(&game->mlx, sizeof(game->mlx));
	ft_bzero(&game->assets, sizeof(game->assets));
	ft_bzero(&game->render, sizeof(game->render));
	game->runtime_mask = 0u;
}

static bool	init_mlx_instance(t_game *game)
{
	game->mlx.mlx = mlx_init();
	if (game->mlx.mlx == NULL)
		return (false);
	game->runtime_mask |= GAME_MLX_READY;
	return (true);
}

static bool	init_window(t_game *game)
{
	game->mlx.win = mlx_new_window(game->mlx.mlx, WIN_W, WIN_H, "cub3d");
	if (game->mlx.win == NULL)
		return (false);
	game->runtime_mask |= GAME_WINDOW_READY;
	return (true);
}

static bool	init_frame_buffer(t_game *game)
{
	t_img	*frame;

	frame = &game->render.frame;
	frame->img = mlx_new_image(game->mlx.mlx, WIN_W, WIN_H);
	if (frame->img == NULL)
		return (false);
	game->runtime_mask |= GAME_FRAME_READY;
	frame->addr = mlx_get_data_addr(frame->img, &frame->bpp,
			&frame->line_len, &frame->endian);
	if (frame->addr == NULL)
		return (false);
	frame->width = WIN_W;
	frame->height = WIN_H;
	return (true);
}

bool	init_game_mlx(t_game *game)
{
	if (game == NULL)
		return (false);
	destroy_game_resources(game);
	reset_runtime_resources(game);
	if (!init_mlx_instance(game) || !init_window(game)
		|| !init_frame_buffer(game))
	{
		destroy_game_resources(game);
		return (false);
	}
	return (true);
}
