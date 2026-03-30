/**
 * @file cast_ray.c
 * @brief DDAアルゴリズムによるレイキャスト処理
 */
#include "render/render_internal.h"
#include "game_config.h"
#include <math.h>

/**
 * @brief 画面列に対するレイの方向と初期マップセルを計算する
 *
 * @param[out] ray    計算結果の書き込み先
 * @param[in]  player プレイヤーの位置と向き
 * @param[in]  col    画面列 (0 <= col < WIN_W)
 */
static void	init_ray_direction(t_ray *ray, const t_player *player, int col)
{
	double	camera_x_step;

	camera_x_step = CAMERA_X_SCALE / (double)(WIN_W - 1);
	ray->column = col;
	ray->camera_x = CAMERA_X_MIN + col * camera_x_step;
	ray->ray_dir.x = player->dir.x + player->plane.x * ray->camera_x;
	ray->ray_dir.y = player->dir.y + player->plane.y * ray->camera_x;
	ray->map.x = (int)player->pos.x;
	ray->map.y = (int)player->pos.y;
}

/**
 * @brief DDAループでグリッドを1セルずつ進み,壁に衝突するまで繰り返す
 *
 * @param[in,out] ray DDA状態 (side_dist/map/hit_sideを更新)
 * @param[in]     map マップデータ
 */
static void	run_dda(t_ray *ray, const t_map *map)
{
	while (true)
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
		if (map_is_solid(map, ray->map.x, ray->map.y))
			return ;
	}
}

/**
 * @brief 魚眼補正済みの壁距離と壁面上の衝突位置を計算する
 *
 * @param[in,out] ray    壁距離と衝突位置の書き込み先
 * @param[in]     player プレイヤーの位置
 */
static void	calc_wall_dist(t_ray *ray, const t_player *player)
{
	if (ray->hit_side == HIT_X)
	{
		ray->perp_wall_dist = ray->side_dist.x - ray->delta_dist.x;
		ray->wall_x = player->pos.y + ray->perp_wall_dist * ray->ray_dir.y;
	}
	else
	{
		ray->perp_wall_dist = ray->side_dist.y - ray->delta_dist.y;
		ray->wall_x = player->pos.x + ray->perp_wall_dist * ray->ray_dir.x;
	}
	if (ray->perp_wall_dist < MIN_PERP_WALL_DIST)
		ray->perp_wall_dist = MIN_PERP_WALL_DIST;
	ray->wall_x -= floor(ray->wall_x);
}

/**
 * @brief 指定画面列に対してDDAレイキャストを実行する
 *
 * レイ方向の初期化 -> DDAパラメータ設定 -> DDAループ -> 壁距離計算
 * の順で,1列分のレイキャスト結果をrayに書き込む。
 *
 * @param[out] ray    レイキャスト結果の書き込み先
 * @param[in]  player プレイヤーの位置と向き
 * @param[in]  map    マップデータ
 * @param[in]  col    画面列 (0 <= col < WIN_W)
 */
void	cast_ray(t_ray *ray, const t_player *player, const t_map *map, int col)
{
	init_ray_direction(ray, player, col);
	init_dda_step(ray, player);
	run_dda(ray, map);
	calc_wall_dist(ray, player);
}
