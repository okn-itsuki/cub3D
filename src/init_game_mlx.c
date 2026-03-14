#include "game_init.h"
#include "libft.h"
#include <stdlib.h>

// 何する関数か:
// - MLX 初期化が所有する実行時フィールドだけを 0 初期化する。
// 参照でいじった値:
// - `game_state->mlx` を 0 で埋める。
// - `game_state->render.frame` を 0 で埋める。
// - `game_state->assets` を 0 で埋める。
// - `game_state->init_mask` から MLX/texture 関連 bit を落とす。
// 戻り値の意味:
// - なし。
static void	reset_game_runtime_resources(t_game *game_state)
{
	ft_bzero(&game_state->mlx, sizeof(game_state->mlx));
	ft_bzero(&game_state->render.frame, sizeof(game_state->render.frame));
	ft_bzero(&game_state->assets, sizeof(game_state->assets));
	game_state->init_mask &= ~(GAME_MLX_READY | GAME_WINDOW_READY
			| GAME_FRAME_READY | GAME_WALL_TEXTURES_READY);
}

// 何する関数か:
// - `init_mask` を見て、初期化済みのゲーム資源を逆順で一括開放する。
// 参照でいじった値:
// - `game_state->assets.wall[]` を必要なら破棄して空状態へ戻す。
// - `game_state->render.frame.img` を破棄して `NULL` に戻す。
// - `game_state->render.frame.addr` を `NULL` に戻す。
// - `game_state->mlx.win` を破棄して `NULL` に戻す。
// - `game_state->mlx.mlx` を破棄して `NULL` に戻す。
// - `game_state->init_mask` から破棄済み bit を落とす。
// 戻り値の意味:
// - なし。
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

// 何する関数か:
// - MLX本体を起動する。
// 参照でいじった値:
// - `game_state->mlx.mlx` に `mlx_init()` の返り値を入れる。
// - `game_state->init_mask` に MLX 初期化済み bit を立てる。
// 戻り値の意味:
// - `true`: MLX本体の起動に成功した。
// - `false`: MLX本体の起動に失敗した。
static bool	init_mlx_instance(t_game *game_state)
{
	game_state->mlx.mlx = mlx_init();
	if (game_state->mlx.mlx == NULL)
		return (false);
	game_state->init_mask |= GAME_MLX_READY;
	return (true);
}

// 何する関数か:
// - 描画先の window を作る。
// 参照でいじった値:
// - `game_state->mlx.win` に `mlx_new_window()` の返り値を入れる。
// - `game_state->init_mask` に window 初期化済み bit を立てる。
// 戻り値の意味:
// - `true`: window の作成に成功した。
// - `false`: window の作成に失敗した。
static bool	init_game_window(t_game *game_state)
{
	game_state->mlx.win = mlx_new_window(game_state->mlx.mlx,
			WIN_W, WIN_H, "cub3d");
	if (game_state->mlx.win == NULL)
		return (false);
	game_state->init_mask |= GAME_WINDOW_READY;
	return (true);
}

// 何する関数か:
// - off-screen描画用の frame画像を作り、pixel書き込み情報を設定する。
// 参照でいじった値:
// - `game_state->render.frame.img` に画像本体を入れる。
// - `game_state->render.frame.addr`, `bpp`, `line_len`, `endian` を設定する。
// - `game_state->render.frame.width`, `height` を画面サイズで設定する。
// - `game_state->init_mask` に frame 初期化済み bit を立てる。
// 戻り値の意味:
// - `true`: frame画像の作成と pixel 情報の取得に成功した。
// - `false`: 途中で失敗した。
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

// 何する関数か:
// - MLX 関連の実行時フィールドを初期化し、MLX本体, window, frame画像を順に作る。
// 参照でいじった値:
// - `game_state->mlx`, `game_state->render.frame`, `game_state->assets` を
//   初期状態へ戻す。
// - `game_state->mlx.mlx`, `game_state->mlx.win` を設定する。
// - `game_state->render.frame` の各項目を設定する。
// - `game_state->init_mask` に各初期化段階の bit を立てる。
// - 失敗したら、`init_mask` を見て初期化済み資源だけを一括開放する。
// 戻り値の意味:
// - `true`: MLX初期化が最後まで完了した。
// - `false`: 途中で失敗した。
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
