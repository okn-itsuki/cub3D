/**
 * @file cast_ray_init.c
 * @brief DDA開始時の距離・進行方向の初期化
 *
 * @details
 * DDAは,開始前に「どちらの軸へ進むか」と「次の境界までどれだけ距離があるか」を
 * 前計算しておく必要がある.このファイルはその初期条件だけに責務を絞っている.
 */
#include "render/render_internal.h"
#include "game_config.h"
#include <math.h>

/**
 * @brief レイ方向1軸分のdelta distanceを計算する
 *
 * `delta_dist`は,その軸方向へ1セル進むのに必要なレイ上の距離.
 * 成分が0のときは,その軸方向の境界へ永遠に到達しない扱いとして疑似無限大を返す.
 *
 * @param[in] ray_dir レイ方向成分
 * @return 1セル進むのに必要な距離
 */
static double	calc_delta_distance(double ray_dir)
{
	if (ray_dir == 0.0)
		return (RAY_DELTA_INF);
	return (fabs(1.0 / ray_dir));
}

/**
 * @brief DDAの1軸分のside_dist,delta_dist,stepを初期化する
 *
 * `step`はマップ上での進行符号,`side_dist`は現在位置から最初の境界までの距離を表す.
 */
static void	init_dda_axis(double pos, int map_pos, double ray_dir,
		double *side_dist, double *delta_dist, int *step)
{
	*delta_dist = calc_delta_distance(ray_dir);
	if (ray_dir < 0.0)
	{
		*step = -1;
		*side_dist = (pos - map_pos) * *delta_dist;
		return ;
	}
	*step = 1;
	*side_dist = (map_pos + 1.0 - pos) * *delta_dist;
}

/**
 * @brief DDAに必要なdelta_dist・step・初期side_distを計算する
 *
 * この初期化が済むと,あとは`run_dda()`側が小さい`side_dist`を選んで
 * 加算していくだけでレイを進められる.
 *
 * @param[in,out] ray    DDAパラメータの書き込み先
 * @param[in]     player プレイヤーの位置
 */
void	init_dda_step(t_ray *ray, const t_player *player)
{
	init_dda_axis(player->pos.x, ray->map.x, ray->ray_dir.x,
		&ray->side_dist.x, &ray->delta_dist.x, &ray->step.x);
	init_dda_axis(player->pos.y, ray->map.y, ray->ray_dir.y,
		&ray->side_dist.y, &ray->delta_dist.y, &ray->step.y);
}
