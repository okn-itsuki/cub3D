#include "game_mlx_init.h"


static void	destroy_game_mlx(t_game *game_state)
{
	if ((game_state->init_mask & GAME_FRAME_READY) != 0u)
	{
		mlx_destroy_image(game_state->mlx.mlx, game_state->render.frame.img);
		game_state->render.frame.img = NULL;
		game_state->render.frame.addr = NULL;
		game_state->init_mask &= ~GAME_FRAME_READY;
	}
	if ((game_state->init_mask & GAME_WINDOW_READY) != 0u)
	{
		mlx_destroy_window(game_state->mlx.mlx, game_state->mlx.win);
		game_state->mlx.win = NULL;
		game_state->init_mask &= ~GAME_WINDOW_READY;
	}
	if ((game_state->init_mask & GAME_MLX_READY) != 0u)
	{
#ifdef __linux__
		mlx_destroy_display(game_state->mlx.mlx);
#endif
		free(game_state->mlx.mlx);
		game_state->mlx.mlx = NULL;
		game_state->init_mask &= ~GAME_MLX_READY;
	}
}

static bool	init_mlx_instance(t_game *game_state)
{
	game_state->mlx.mlx = mlx_init();
	if (game_state->mlx.mlx == NULL)
		return (false);
	game_state->init_mask |= GAME_MLX_READY;
	return (true);
}

static bool	init_game_window(t_game *game_state)
{
	game_state->mlx.win = mlx_new_window(game_state->mlx.mlx,
			WIN_W, WIN_H, "cub3d");
	if (game_state->mlx.win == NULL)
		return (false);
	game_state->init_mask |= GAME_WINDOW_READY;
	return (true);
}

static bool	init_frame_buffer(t_game *game_state)
{
	t_img	*frame_buffer;

	frame_buffer = &game_state->render.frame;
	frame_buffer->img = mlx_new_image(game_state->mlx.mlx, WIN_W, WIN_H);
	if (frame_buffer->img == NULL)
		return (false);
	game_state->init_mask |= GAME_FRAME_READY;
	frame_buffer->addr = mlx_get_data_addr(frame_buffer->img, &frame_buffer->bpp,
			&frame_buffer->line_len, &frame_buffer->endian);
	if (frame_buffer->addr == NULL)
		return (false);
	frame_buffer->width = WIN_W;
	frame_buffer->height = WIN_H;
	return (true);
}

bool	init_game_mlx(t_game *game_state)
{
	ft_memset(game_state, 0, sizeof(*game_state));
	if (init_mlx_instance(game_state) == false)
		return (false);
	if (init_game_window(game_state) == false)
	{
		destroy_game_mlx(game_state);
		return (false);
	}
	if (init_frame_buffer(game_state) == false)
	{
		destroy_game_mlx(game_state);
		return (false);
	}
	return (true);
}
