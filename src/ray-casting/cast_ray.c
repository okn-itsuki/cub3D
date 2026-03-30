/**
 * @file cast_ray.c
 * @brief DDAアルゴリズムによるレイキャスト処理
 */
#include "ray_casting.h"
#include "game_config.h"
#include <math.h>

/**
 * @brief 画面列に対するレイの方向と初期マップセルを計算する
 *
 * カメラ平面上の正規化座標からレイ方向を求め、
 * プレイヤー位置から初期マップセルを設定する。
 *
 * @param[out] ray    計算結果の書き込み先
 * @param[in]  player プレイヤーの位置と向き
 * @param[in]  col    画面列 (0 <= col < WIN_W)
 */
static void	init_ray_direction(t_ray *ray, const t_player *player, int col)
{
	const double	camera_x_step = CAMERA_X_SCALE / (double)(WIN_W - 1);

	ray->column = col;
	ray->camera_x = CAMERA_X_MIN + col * camera_x_step;
	ray->ray_dir.x = player->dir.x + player->plane.x * ray->camera_x;
	ray->ray_dir.y = player->dir.y + player->plane.y * ray->camera_x;
	ray->map.x = (int)player->pos.x;
	ray->map.y = (int)player->pos.y;
}

/**
 * @brief DDAに必要なdelta_dist・step・初期side_distを計算する
 *
 * delta_dist: 各軸方向に1セル進むのに必要な距離。
 * step: 各軸の進行方向 (-1 or +1)。
 * side_dist: 現在位置から最初のグリッド境界までの距離。
 *
 * @param[in,out] ray    DDAパラメータの書き込み先
 * @param[in]     player プレイヤーの位置
 */
static void	init_dda_step(t_ray *ray, const t_player *player)
{
	ray->delta_dist.x = (ray->ray_dir.x == 0)
		* RAY_DELTA_INF + (ray->ray_dir.x != 0)
		* fabs(1.0 / ray->ray_dir.x);
	ray->delta_dist.y = (ray->ray_dir.y == 0)
		* RAY_DELTA_INF + (ray->ray_dir.y != 0)
		* fabs(1.0 / ray->ray_dir.y);
	if (ray->ray_dir.x < 0)
	{
		ray->step.x = -1;
		ray->side_dist.x = (player->pos.x - ray->map.x) * ray->delta_dist.x;
	}
	else
	{
		ray->step.x = 1;
		ray->side_dist.x = (ray->map.x + 1.0 - player->pos.x)
			* ray->delta_dist.x;
	}
	if (ray->ray_dir.y < 0)
	{
		ray->step.y = -1;
		ray->side_dist.y = (player->pos.y - ray->map.y) * ray->delta_dist.y;
	}
	else
	{
		ray->step.y = 1;
		ray->side_dist.y = (ray->map.y + 1.0 - player->pos.y)
			* ray->delta_dist.y;
	}
}

/**
 * @brief DDAループでグリッドを1セルずつ進み、壁に衝突するまで繰り返す
 *
 * side_distが小さい方の軸を選んで1セル進める。
 * マップ範囲外に出るか壁セル('1')に到達したら停止する。
 *
 * @param[in,out] ray DDA状態 (side_dist/map/hit_sideを更新)
 * @param[in]     map マップデータ (境界チェックと壁判定に使用)
 */
static void	run_dda(t_ray *ray, const t_map *map)
{
	while (1)
	{
		if (ray->side_dist.x < ray->side_dist.y)
		{
			ray->side_dist.x += ray->delta_dist.x;
			ray->map.x += ray->step.x;
			ray->hit_side = HIT_X;
		}
		else
		{
			ray->side_dist.y += ray->delta_dist.y;
			ray->map.y += ray->step.y;
			ray->hit_side = HIT_Y;
		}
		if (ray->map.x < 0 || ray->map.x >= map->width
			|| ray->map.y < 0 || ray->map.y >= map->height)
			break ;
		if (map->grid[ray->map.y][ray->map.x] == MAP_WALL_CELL)
			break ;
	}
}

/**
 * @brief 魚眼補正済みの壁距離と壁面上の衝突位置を計算する
 *
 * perp_wall_dist: カメラ平面に垂直な壁までの距離 (魚眼補正済み)。
 * wall_x: 衝突位置の壁面上での小数部 (0.0 <= wall_x < 1.0)。
 * テクスチャのU座標として使用する。
 *
 * @param[in,out] ray    壁距離と衝突位置の書き込み先
 * @param[in]     player プレイヤーの位置
 */
static void	calc_wall_dist(t_ray *ray, const t_player *player)
{
	if (ray->hit_side == HIT_X)
	{
		ray->perp_wall_dist = ray->side_dist.x - ray->delta_dist.x;
		if (ray->perp_wall_dist < MIN_PERP_WALL_DIST)
			ray->perp_wall_dist = MIN_PERP_WALL_DIST;
		ray->wall_x = player->pos.y
			+ ray->perp_wall_dist * ray->ray_dir.y;
	}
	else
	{
		ray->perp_wall_dist = ray->side_dist.y - ray->delta_dist.y;
		if (ray->perp_wall_dist < MIN_PERP_WALL_DIST)
			ray->perp_wall_dist = MIN_PERP_WALL_DIST;
		ray->wall_x = player->pos.x
			+ ray->perp_wall_dist * ray->ray_dir.x;
	}
	ray->wall_x = ray->wall_x - floor(ray->wall_x);
}

/**
 * @brief 指定画面列に対してDDAレイキャストを実行する
 *
 * レイ方向の初期化 -> DDAパラメータ設定 -> DDAループ -> 壁距離計算
 * の4ステップで、1列分のレイキャスト結果をrayに書き込む。
 *
 * @param[out] ray    レイキャスト結果の書き込み先
 * @param[in]  player プレイヤーの位置と向き
 * @param[in]  map    マップデータ
 * @param[in]  col    画面列 (0 <= col < WIN_W)
 */
void	cast_ray(t_ray *ray, const t_player *player, const t_map *map,
		int col)
{
	init_ray_direction(ray, player, col);
	init_dda_step(ray, player);
	run_dda(ray, map);
	calc_wall_dist(ray, player);
}
