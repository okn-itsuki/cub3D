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



bool	init_game_mlx(t_game *game_state);

#endif
