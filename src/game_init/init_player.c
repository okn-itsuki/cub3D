#include "game_init.h"

#define PLAYER_MOVE_SPEED 3.0
#define PLAYER_ROT_SPEED 2.0
#define FOV_HALF_TAN 0.66

// 何する関数か:
// - 方角に対応する前方ベクトルを返す。
// 参照でいじった値:
// - なし。
// 戻り値の意味:
// - `dir` に対応する単位方向ベクトル。
static t_vec2d	get_dir_vector(t_dir dir)
{
	const t_vec2d	table[] = {
	[EAST] = {1.0, 0.0},
	[WEST] = {-1.0, 0.0},
	[NORTH] = {0.0, -1.0},
	[SOUTH] = {0.0, 1.0}
	};

	return (table[dir]);
}

// 何する関数か:
// - 方角に対応するカメラ平面ベクトルを返す。
// 参照でいじった値:
// - なし。
// 戻り値の意味:
// - `dir` に対応するカメラ平面ベクトル。
//   + `dir` と直交し、長さは `FOV_HALF_TAN` (FOV ≈ 66°)。
static t_vec2d	get_plane_vector(t_dir dir)
{
	const t_vec2d	table[] = {
	[EAST] = {0.0, FOV_HALF_TAN},
	[WEST] = {0.0, -FOV_HALF_TAN},
	[NORTH] = {FOV_HALF_TAN, 0.0},
	[SOUTH] = {-FOV_HALF_TAN, 0.0}
	};

	return (table[dir]);
}

// 何する関数か:
// - `t_spawn` の情報から `t_player` の全フィールドを初期化する。
// 参照でいじった値:
// - `player->pos` をセル中心座標に設定する。
// - `player->dir` を方角に対応する単位ベクトルに設定する。
// - `player->plane` を方角に対応するカメラ平面ベクトルに設定する。
// - `player->move_speed` を秒速 `PLAYER_MOVE_SPEED` に設定する。
// - `player->rot_speed` を秒速 `PLAYER_ROT_SPEED` に設定する。
// 戻り値の意味:
// - なし。
void	init_player(t_player *player, t_spawn spawn)
{
	player->pos.x = spawn.col + 0.5;
	player->pos.y = spawn.row + 0.5;
	player->dir = get_dir_vector(spawn.dir);
	player->plane = get_plane_vector(spawn.dir);
	player->move_speed = PLAYER_MOVE_SPEED;
	player->rot_speed = PLAYER_ROT_SPEED;
}
