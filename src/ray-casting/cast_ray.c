#include "ray_casting.h"
#include "game_config.h"
#include <math.h>

// 何する関数か:
// - 画面列 `col` に対する ray の方向と初期 map cell を計算する。
// 参照でいじった値:
// - `ray->column`, `ray->camera_x`, `ray->ray_dir`, `ray->map` を設定する。
// 戻り値の意味:
// - なし。
static void	init_ray_direction(t_ray *ray, const t_player *player, int col)
{
	ray->column = col;
	ray->camera_x = CAMERA_X_SCALE * col / (WIN_W - 1) + CAMERA_X_MIN;
	ray->ray_dir.x = player->dir.x + player->plane.x * ray->camera_x;
	ray->ray_dir.y = player->dir.y + player->plane.y * ray->camera_x;
	ray->map.x = (int)player->pos.x;
	ray->map.y = (int)player->pos.y;
}

// 何する関数か:
// - DDA に必要な `delta_dist`, `step`, 初期 `side_dist` を計算する。
// 参照でいじった値:
// - `ray->delta_dist`, `ray->step`, `ray->side_dist` を設定する。
// 戻り値の意味:
// - なし。
static void	init_dda_step(t_ray *ray, const t_player *player)
{
	if (ray->ray_dir.x == 0)
		ray->delta_dist.x = RAY_DELTA_INF;
	else
		ray->delta_dist.x = fabs(1.0 / ray->ray_dir.x);
	if (ray->ray_dir.y == 0)
		ray->delta_dist.y = RAY_DELTA_INF;
	else
		ray->delta_dist.y = fabs(1.0 / ray->ray_dir.y);
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

// 何する関数か:
// - DDA ループで grid を1セルずつ進み、壁 '1' に衝突するまで繰り返す。
// 参照でいじった値:
// - `ray->side_dist`, `ray->map`, `ray->hit_side` を更新する。
// 戻り値の意味:
// - なし。
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
		if (map->grid[ray->map.y][ray->map.x] == MAP_WALL_CELL)
			break ;
	}
}

// 何する関数か:
// - 魚眼補正済みの壁までの垂直距離と壁面上の衝突位置を計算する。
// 参照でいじった値:
// - `ray->perp_wall_dist`, `ray->wall_x` を設定する。
// 戻り値の意味:
// - なし。
static void	calc_wall_dist(t_ray *ray, const t_player *player)
{
	if (ray->hit_side == HIT_X)
		ray->perp_wall_dist = ray->side_dist.x - ray->delta_dist.x;
	else
		ray->perp_wall_dist = ray->side_dist.y - ray->delta_dist.y;
	if (ray->perp_wall_dist < MIN_PERP_WALL_DIST)
		ray->perp_wall_dist = MIN_PERP_WALL_DIST;
	if (ray->hit_side == HIT_X)
		ray->wall_x = player->pos.y
			+ ray->perp_wall_dist * ray->ray_dir.y;
	else
		ray->wall_x = player->pos.x
			+ ray->perp_wall_dist * ray->ray_dir.x;
	ray->wall_x = ray->wall_x - floor(ray->wall_x);
}

// 何する関数か:
// - 画面列 `col` に対して DDA レイキャストを実行し、結果を `ray` に書き込む。
// 参照でいじった値:
// - `ray` の全フィールドを設定する。
// 戻り値の意味:
// - なし。
void	cast_ray(t_ray *ray, const t_player *player, const t_map *map,
		int col)
{
	init_ray_direction(ray, player, col);
	init_dda_step(ray, player);
	run_dda(ray, map);
	calc_wall_dist(ray, player);
}
