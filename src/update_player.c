/**
 * @file update_player.c
 * @brief プレイヤーの回転・移動・壁衝突判定
 */
#include "ray_casting.h"
#include "game_config.h"
#include <math.h>

/**
 * @brief プレイヤーの方向ベクトルとカメラ平面を回転する
 *
 * 2D回転行列を適用し、dirとplaneを同時に回転させる。
 *
 * @param[in,out] player 回転対象のプレイヤー (dir/planeを更新)
 * @param[in]     angle  回転角 [rad] (正=右回り、負=左回り)
 */
static void	rotate_player(t_player *player, double angle)
{
	double	cos_a;
	double	sin_a;
	t_vec2d	old_dir;
	t_vec2d	old_plane;

	cos_a = cos(angle);
	sin_a = sin(angle);
	old_dir = player->dir;
	old_plane = player->plane;
	player->dir.x = old_dir.x * cos_a - old_dir.y * sin_a;
	player->dir.y = old_dir.x * sin_a + old_dir.y * cos_a;
	player->plane.x = old_plane.x * cos_a - old_plane.y * sin_a;
	player->plane.y = old_plane.x * sin_a + old_plane.y * cos_a;
}

/**
 * @brief ワールド座標が壁セルかどうかを判定する
 *
 * @param[in] map マップデータ
 * @param[in] x   判定するX座標
 * @param[in] y   判定するY座標
 * @retval true  壁またはマップ範囲外
 * @retval false 通行可能
 */
static bool	is_wall(const t_map *map, double x, double y)
{
	int	ix;
	int	iy;

	ix = (int)x;
	iy = (int)y;
	if (ix < 0 || ix >= map->width || iy < 0 || iy >= map->height)
		return (true);
	return (map->grid[iy][ix] == MAP_WALL_CELL);
}

/**
 * @brief 壁との衝突判定付きでプレイヤーを移動する
 *
 * X軸とY軸を独立に判定することで、壁沿いスライド移動を実現する。
 * 各軸で移動先+マージン位置が壁でなければ移動を適用する。
 *
 * @param[in,out] player 移動するプレイヤー (posを更新)
 * @param[in]     map    壁判定用のマップデータ
 * @param[in]     delta  移動量ベクトル
 */
static void	move_with_collision(t_player *player, const t_map *map,
		t_vec2d delta)
{
	double	margin_x;
	double	margin_y;
	double	new_x;
	double	new_y;

	if (delta.x > 0)
		margin_x = COLLISION_MARGIN;
	else
		margin_x = -COLLISION_MARGIN;
	if (delta.y > 0)
		margin_y = COLLISION_MARGIN;
	else
		margin_y = -COLLISION_MARGIN;
	new_x = player->pos.x + delta.x;
	new_y = player->pos.y + delta.y;
	if (!is_wall(map, new_x + margin_x, player->pos.y + COLLISION_MARGIN)
		&& !is_wall(map, new_x + margin_x, player->pos.y - COLLISION_MARGIN))
		player->pos.x = new_x;
	if (!is_wall(map, player->pos.x + COLLISION_MARGIN, new_y + margin_y)
		&& !is_wall(map, player->pos.x - COLLISION_MARGIN, new_y + margin_y))
		player->pos.y = new_y;
}

/**
 * @brief 入力状態とデルタタイムからプレイヤーを1フレーム分更新する
 *
 * 回転入力 -> 移動入力の順で処理する。
 * 横移動(strafe)はカメラ平面方向の正規化ベクトルを使用する。
 * 移動量がゼロでなければ壁衝突判定付きで移動を適用する。
 *
 * @param[in,out] player プレイヤー状態 (pos/dir/planeを更新)
 * @param[in]     input  現在の入力状態
 * @param[in]     map    壁判定用のマップデータ
 * @param[in]     dt     前フレームからの経過時間 [sec]
 */
void	update_player(t_player *player, const t_input *input,
		const t_map *map, double dt)
{
	t_vec2d	delta;
	double	plane_len;

	if (input->turn_left)
		rotate_player(player, -player->rot_speed * dt);
	if (input->turn_right)
		rotate_player(player, player->rot_speed * dt);
	delta = (t_vec2d){0, 0};
	if (input->move_forward)
	{
		delta.x += player->dir.x * player->move_speed * dt;
		delta.y += player->dir.y * player->move_speed * dt;
	}
	if (input->move_backward)
	{
		delta.x -= player->dir.x * player->move_speed * dt;
		delta.y -= player->dir.y * player->move_speed * dt;
	}
	plane_len = sqrt(player->plane.x * player->plane.x
			+ player->plane.y * player->plane.y);
	if (input->strafe_left && plane_len > 0)
	{
		delta.x -= (player->plane.x / plane_len) * player->move_speed * dt;
		delta.y -= (player->plane.y / plane_len) * player->move_speed * dt;
	}
	if (input->strafe_right && plane_len > 0)
	{
		delta.x += (player->plane.x / plane_len) * player->move_speed * dt;
		delta.y += (player->plane.y / plane_len) * player->move_speed * dt;
	}
	if (delta.x != 0 || delta.y != 0)
		move_with_collision(player, map, delta);
}
