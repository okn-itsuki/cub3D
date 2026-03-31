#include "game_init/game_init_internal.h"
#include "libft.h"

static void	destroy_frame(t_game *game)
{
	if ((game->runtime_mask & GAME_FRAME_READY) == 0u)
		return ;
	mlx_destroy_image(game->mlx.mlx, game->render.frame.img);
	ft_bzero(&game->render.frame, sizeof(game->render.frame));
	game->runtime_mask &= ~GAME_FRAME_READY;
}

static void	destroy_window(t_game *game)
{
	if ((game->runtime_mask & GAME_WINDOW_READY) == 0u)
		return ;
	mlx_destroy_window(game->mlx.mlx, game->mlx.win);
	game->mlx.win = NULL;
	game->runtime_mask &= ~GAME_WINDOW_READY;
}

static void	destroy_mlx(t_game *game)
{
	if ((game->runtime_mask & GAME_MLX_READY) == 0u)
		return ;
	destroy_mlx_display(game->mlx.mlx);
	game->mlx.mlx = NULL;
	game->runtime_mask &= ~GAME_MLX_READY;
}

void	destroy_game_resources(t_game *game)
{
	if (game == NULL)
		return ;
	if ((game->runtime_mask & GAME_WALL_TEXTURES_READY) != 0u)
	{
		destroy_texture_assets(&game->assets, game->mlx);
		game->runtime_mask &= ~GAME_WALL_TEXTURES_READY;
	}
	destroy_frame(game);
	destroy_window(game);
	destroy_mlx(game);
}
