/**
 * @file update_player.c
 * @brief プレイヤーの回転・移動・壁衝突判定
 *
 * @details
 * このモジュールは,入力状態を「回転」と「平面移動」へ分解してプレイヤーへ適用する.
 * 回転では`dir`と`plane`を同じ回転行列で更新し,
 * 移動では前後方向とストレイフ方向を合成してから衝突判定付きで反映する.
 * 数学的には,ベクトル合成,正規化,軸分離衝突判定が中心になる.
 */
#include "ray_casting.h"
#include "game_config.h"
#include <math.h>

/**
 * @brief プレイヤーの方向ベクトルとカメラ平面を回転する
 *
 * @param[in,out] player 回転対象のプレイヤー (dir/planeを更新)
 * @param[in]     angle  回転角 [rad] (正=右回り,負=左回り)
 */
void	rotate_player(t_player *player, double angle)
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
 * @brief ワールド座標が移動不能セルかどうかを判定する
 *
 * 範囲外やvoid領域も`map_is_solid()`側で壁として扱う。
 *
 * @param[in] map マップデータ
 * @param[in] x   判定するX座標
 * @param[in] y   判定するY座標
 * @retval true  移動不能
 * @retval false 通行可能
 */
static bool	is_wall(const t_map *map, double x, double y)
{
	return (map_is_solid(map, (int)x, (int)y));
}

/**
 * @brief ベクトルに別ベクトルのscale倍を加算する
 *
 * 前進/後退/左右移動の寄与を,ひとつの`delta`へ逐次合成するための補助関数.
 *
 * @param[in,out] dst   加算先
 * @param[in]     axis  加算元ベクトル
 * @param[in]     scale 乗数
 */
static void	add_scaled_vector(t_vec2d *dst, t_vec2d axis, double scale)
{
	dst->x += axis.x * scale;
	dst->y += axis.y * scale;
}

/**
 * @brief カメラ平面からストレイフ方向ベクトルを求める
 *
 * `plane`は視野角スケール`tan(FOV/2)`を含むため,
 * そのままではストレイフ速度へFOV依存が混ざる.
 * そこで`FOV_HALF_TAN`で割り,長さ1の横方向ベクトルへ戻している.
 *
 * @param[in] player プレイヤー状態
 * @return 正規化済みの平行移動方向ベクトル
 */
static t_vec2d	get_strafe_axis(const t_player *player)
{
	return ((t_vec2d){
		player->plane.x / FOV_HALF_TAN,
		player->plane.y / FOV_HALF_TAN
	});
}

/**
 * @brief 壁との衝突判定付きでプレイヤーを移動する
 *
 * X軸とY軸を独立に判定することで,壁沿いスライド移動を実現する。
 * 片方の軸だけが塞がれていても,もう片方の軸の移動は残せるため,
 * 斜め移動時に「完全停止」ではなく「壁に沿って流れる」挙動になる.
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

	margin_x = COLLISION_MARGIN;
	if (delta.x < 0)
		margin_x = -margin_x;
	margin_y = COLLISION_MARGIN;
	if (delta.y < 0)
		margin_y = -margin_y;
	if (!is_wall(map, player->pos.x + delta.x + margin_x,
			player->pos.y + COLLISION_MARGIN)
		&& !is_wall(map, player->pos.x + delta.x + margin_x,
			player->pos.y - COLLISION_MARGIN))
		player->pos.x += delta.x;
	if (!is_wall(map, player->pos.x + COLLISION_MARGIN,
			player->pos.y + delta.y + margin_y)
		&& !is_wall(map, player->pos.x - COLLISION_MARGIN,
			player->pos.y + delta.y + margin_y))
		player->pos.y += delta.y;
}

/**
 * @brief 入力状態から1フレーム分の移動ベクトルを組み立てる
 *
 * 斜め移動時は長さを正規化して,通常移動より速くならないよう補正する。
 * これにより,W+Dのような複合入力でも,単独入力と同じ速度上限を保てる.
 *
 * @param[out]    delta  組み立てた移動量
 * @param[in]     player プレイヤー状態
 * @param[in]     input  現在の入力状態
 * @param[in]     dt     前フレームからの経過時間 [sec]
 */
static void	build_move_delta(t_vec2d *delta, const t_player *player,
		const t_input *input, double dt)
{
	double	speed;
	t_vec2d	strafe_axis;
	double	delta_len;
	double	scale;

	speed = player->move_speed * dt;
	if (input->move_forward)
		add_scaled_vector(delta, player->dir, speed);
	if (input->move_backward)
		add_scaled_vector(delta, player->dir, -speed);
	strafe_axis = get_strafe_axis(player);
	if (input->strafe_left)
		add_scaled_vector(delta, strafe_axis, -speed);
	if (input->strafe_right)
		add_scaled_vector(delta, strafe_axis, speed);
	delta_len = sqrt(delta->x * delta->x + delta->y * delta->y);
	if (delta_len > speed && delta_len > 0.0)
	{
		scale = speed / delta_len;
		delta->x *= scale;
		delta->y *= scale;
	}
}

/**
 * @brief 入力状態とデルタタイムからプレイヤーを1フレーム分更新する
 *
 * 回転入力を反映した後,移動ベクトルを組み立て,壁衝突判定付きで移動を適用する。
 * `dt`を掛けているため,フレームレートが多少変動しても移動量は時間基準で安定する.
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

	if (input->turn_left)
		rotate_player(player, -player->rot_speed * dt);
	if (input->turn_right)
		rotate_player(player, player->rot_speed * dt);
	delta = (t_vec2d){0.0, 0.0};
	build_move_delta(&delta, player, input, dt);
	if (delta.x != 0.0 || delta.y != 0.0)
		move_with_collision(player, map, delta);
}
