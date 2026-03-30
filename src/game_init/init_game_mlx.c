/**
 * @file init_game_mlx.c
 * @brief MLX本体・ウィンドウ・フレームバッファの初期化と破棄
 */
#include "game_init.h"
#include "libft.h"
#include <stdlib.h>

/**
 * @brief MLX関連の実行時フィールドをゼロ初期化する
 *
 * mlx, frame, assetsをゼロクリアし、init_maskからMLX/texture関連ビットを落とす。
 *
 * @param[in,out] game_state ゼロ初期化するゲーム状態
 */
static void	reset_game_runtime_resources(t_game *game_state)
{
	ft_bzero(&game_state->mlx, sizeof(game_state->mlx));
	ft_bzero(&game_state->render.frame, sizeof(game_state->render.frame));
	ft_bzero(&game_state->assets, sizeof(game_state->assets));
	game_state->init_mask &= ~(GAME_MLX_READY | GAME_WINDOW_READY
			| GAME_FRAME_READY | GAME_WALL_TEXTURES_READY);
}

/**
 * @brief 初期化済みのゲーム資源を逆順で一括解放する
 *
 * init_maskを参照し、初期化済みの資源だけを安全に破棄する。
 * 解放順: テクスチャ -> フレーム -> ウィンドウ -> MLX本体
 *
 * @param[in,out] game_state 解放対象のゲーム状態 (NULLなら何もしない)
 */
void	destroy_game_resources(t_game *game_state)
{
	if (game_state == NULL)
		return ;
	if ((game_state->init_mask & GAME_WALL_TEXTURES_READY) != 0u)
	{
		destroy_texture_assets(&game_state->assets, game_state->mlx);
		game_state->init_mask &= ~GAME_WALL_TEXTURES_READY;
	}
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

/**
 * @brief MLX本体 (mlx_init) を起動する
 *
 * @param[in,out] game_state MLXインスタンスを格納するゲーム状態
 * @retval true  MLX本体の起動に成功
 * @retval false MLX本体の起動に失敗
 */
static bool	init_mlx_instance(t_game *game_state)
{
	game_state->mlx.mlx = mlx_init();
	if (game_state->mlx.mlx == NULL)
		return (false);
	game_state->init_mask |= GAME_MLX_READY;
	return (true);
}

/**
 * @brief 描画先のウィンドウを作成する
 *
 * @param[in,out] game_state ウィンドウを格納するゲーム状態
 * @retval true  ウィンドウの作成に成功
 * @retval false ウィンドウの作成に失敗
 */
static bool	init_game_window(t_game *game_state)
{
	game_state->mlx.win = mlx_new_window(game_state->mlx.mlx,
			WIN_W, WIN_H, "cub3d");
	if (game_state->mlx.win == NULL)
		return (false);
	game_state->init_mask |= GAME_WINDOW_READY;
	return (true);
}

/**
 * @brief オフスクリーン描画用のフレームバッファを作成する
 *
 * MLX画像を生成し、ピクセル直接書き込み用のアドレス情報を取得する。
 *
 * @param[in,out] game_state フレームバッファを格納するゲーム状態
 * @retval true  フレームバッファの作成とピクセル情報の取得に成功
 * @retval false 途中で失敗
 */
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

/**
 * @brief MLX本体・ウィンドウ・フレームバッファを順に初期化する
 *
 * 途中で失敗した場合、初期化済み資源を一括解放してfalseを返す。
 *
 * @param[in,out] game_state 初期化対象のゲーム状態 (NULLなら即false)
 * @retval true  MLX初期化が最後まで完了
 * @retval false 途中で失敗 (資源は解放済み)
 */
bool	init_game_mlx(t_game *game_state)
{
	if (game_state == NULL)
		return (false);
	reset_game_runtime_resources(game_state);
	if (init_mlx_instance(game_state) == false)
		return (false);
	if (init_game_window(game_state) == false)
	{
		destroy_game_resources(game_state);
		return (false);
	}
	if (init_frame_buffer(game_state) == false)
	{
		destroy_game_resources(game_state);
		return (false);
	}
	return (true);
}
