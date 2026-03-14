#ifndef GAME_INIT_H
#define GAME_INIT_H

#include "cub3d.h"

// 何する定義か:
// - `t_game.init_mask` でゲーム資源の初期化進行状況を管理するための bit 定義。
// 参照でいじった値:
// - `game_state->init_mask` の各 bit を立てたり落としたりするときに使う。
// 戻り値の意味:
// - なし。
typedef enum e_game_init_mask
{
	GAME_MLX_READY = 1u << 0,
	GAME_WINDOW_READY = 1u << 1,
	GAME_FRAME_READY = 1u << 2,
	GAME_WALL_TEXTURES_READY = 1u << 3
}	t_game_init_mask;

bool	init_game_mlx(t_game *game_state);
bool	init_game_wall_textures(t_game *game_state, t_tex_path texture_paths);
void	destroy_texture_assets(t_assets *assets, t_mlx mlx_context);
void	destroy_game_resources(t_game *game_state);

#endif
