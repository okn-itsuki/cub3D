#include "cub3d.h"
#include "libft.h"


static void	destroy_game_mlx_resources(t_game *game_state)
{
	if (game_state == NULL)
		return ;
	if (game_state->render.frame.img != NULL)
	{
		mlx_destroy_image(game_state->mlx.mlx, game_state->render.frame.img);
		game_state->render.frame.img = NULL;
		game_state->render.frame.addr = NULL;
	}
	if (game_state->mlx.win != NULL)
	{
		mlx_destroy_window(game_state->mlx.mlx, game_state->mlx.win);
		game_state->mlx.win = NULL;
	}
	if (game_state->mlx.mlx != NULL)
	{
#ifdef __linux__
		mlx_destroy_display(game_state->mlx.mlx);
#endif
		free(game_state->mlx.mlx);
		game_state->mlx.mlx = NULL;
	}
}


static bool	create_mlx_window(t_mlx *mlx_state)
{
	mlx_state->mlx = mlx_init();
	if (mlx_state->mlx == NULL)
		return (false);
	mlx_state->win = mlx_new_window(mlx_state->mlx, WIN_W, WIN_H, "cub3d");
	if (mlx_state->win == NULL)
		return (false);
	return (true);
}


static bool	create_frame_buffer(t_img *frame_buffer, const t_mlx *mlx_state)
{
	frame_buffer->img = mlx_new_image(mlx_state->mlx, WIN_W, WIN_H);
	if (frame_buffer->img == NULL)
		return (false);
	frame_buffer->addr = mlx_get_data_addr(frame_buffer->img, &frame_buffer->bpp,
			&frame_buffer->line_len, &frame_buffer->endian);
	if (frame_buffer->addr == NULL)
	{
		mlx_destroy_image(mlx_state->mlx, frame_buffer->img);
		frame_buffer->img = NULL;
		return (false);
	}
	frame_buffer->width = WIN_W;
	frame_buffer->height = WIN_H;
	return (true);
}

bool	init_game_mlx(t_game *game_state)
{
	ft_memset(game_state, 0, sizeof(*game_state));
	if (create_mlx_window(&game_state->mlx) == false)
	{
		destroy_game_mlx_resources(game_state);
		return (false);
	}
	if (create_frame_buffer(&game_state->render.frame, &game_state->mlx) == false)
	{
		destroy_game_mlx_resources(game_state);
		return (false);
	}
	return (true);
}
