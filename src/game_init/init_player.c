/**
 * @file init_player.c
 * @brief プレイヤーの初期状態設定 (位置・方向・カメラ平面)
 */
#include "game_init.h"
#include "game_config.h"

/**
 * @brief 方角に対応する前方単位ベクトルを返す
 *
 * @param[in] dir 方角 (EAST/WEST/NORTH/SOUTH)
 * @return 方角に対応する単位方向ベクトル
 */
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

/**
 * @brief 方角に対応するカメラ平面ベクトルを返す
 *
 * 前方ベクトルと直交し、長さは FOV_HALF_TAN (FOV約66度)。
 *
 * @param[in] dir 方角 (EAST/WEST/NORTH/SOUTH)
 * @return 方角に対応するカメラ平面ベクトル
 */
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

/**
 * @brief スポーン情報からプレイヤーの全フィールドを初期化する
 *
 * 位置はセルの中心 (col+0.5, row+0.5) に配置する。
 *
 * @param[out] player 初期化するプレイヤー構造体
 * @param[in]  spawn  スポーン位置と方角
 */
void	init_player(t_player *player, t_spawn spawn)
{
	player->pos.x = spawn.col + 0.5;
	player->pos.y = spawn.row + 0.5;
	player->dir = get_dir_vector(spawn.dir);
	player->plane = get_plane_vector(spawn.dir);
	player->move_speed = PLAYER_MOVE_SPEED;
	player->rot_speed = PLAYER_ROT_SPEED;
}
