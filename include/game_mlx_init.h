#ifndef GAME_MLX_INIT_H
#define GAME_MLX_INIT_H

#include "cub3d.h"

// 何する定義か:
// - `t_game.init_mask` で MLX 初期化の進行状況を管理するための bit 定義。
// 参照でいじった値:
// - `game_state->init_mask` の各 bit を立てたり落としたりするときに使う。
// 戻り値の意味:
// - なし。
typedef enum e_game_mlx_init_mask
{
	GAME_MLX_READY = 1u << 0,
	GAME_WINDOW_READY = 1u << 1,
	GAME_FRAME_READY = 1u << 2
}	t_game_mlx_init_mask;

// 何する関数か:
// - `t_game` の MLX 関連資源を初期化する。
// 参照でいじった値:
// - `game_state->mlx.mlx`, `game_state->mlx.win` を設定する。
// - `game_state->render.frame` の各項目を設定する。
// - `game_state->init_mask` に初期化済み bit を立てる。
// 戻り値の意味:
// - `true`: MLX 初期化が最後まで完了した。
// - `false`: 途中で失敗した。
bool	init_game_mlx(t_game *game_state);

#endif
