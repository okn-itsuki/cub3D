/**
 * @file game_runtime_destroy_bonus.c
 * @brief 実行時ゲーム資源の破棄
 */
#include "game_init/game_init_internal_bonus.h"
#include "libft.h"
#include "mouse_bonus.h"

/**
 * @brief フレームバッファを破棄する
 */
static void	destroy_frame(t_game *game)
{
	if ((game->runtime_mask & GAME_FRAME_READY) == 0u)
		return ;
	mlx_destroy_image(game->mlx.mlx, game->render.frame.img);
	ft_bzero(&game->render.frame, sizeof(game->render.frame));
	game->runtime_mask &= ~GAME_FRAME_READY;
}

/**
 * @brief ウィンドウを破棄する
 */
static void	destroy_window(t_game *game)
{
	if ((game->runtime_mask & GAME_WINDOW_READY) == 0u)
		return ;
	mlx_destroy_window(game->mlx.mlx, game->mlx.win);
	game->mlx.win = NULL;
	game->runtime_mask &= ~GAME_WINDOW_READY;
}

/**
 * @brief MLX本体を破棄する
 */
static void	destroy_mlx(t_game *game)
{
	if ((game->runtime_mask & GAME_MLX_READY) == 0u)
		return ;
	destroy_mlx_display(game->mlx.mlx);
	game->mlx.mlx = NULL;
	game->runtime_mask &= ~GAME_MLX_READY;
}

/**
 * @brief 初期化済みのゲーム実行時資源を逆順で一括解放する
 *
 * @param[in,out] game 解放対象のゲーム状態
 */
void	destroy_game_resources(t_game *game)
{
	if (game == NULL)
		return ;
	mouse_release(game);
	if ((game->runtime_mask & GAME_WALL_TEXTURES_READY) != 0u)
	{
		destroy_texture_assets(&game->assets, game->mlx);
		game->runtime_mask &= ~GAME_WALL_TEXTURES_READY;
	}
	destroy_frame(game);
	destroy_window(game);
	destroy_mlx(game);
}
