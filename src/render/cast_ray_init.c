#include "render/render_internal.h"
#include "game_config.h"
#include <math.h>

static double	calc_delta_distance(double ray_dir)
{
	if (ray_dir == 0.0)
		return (RAY_DELTA_INF);
	return (fabs(1.0 / ray_dir));
}

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

void	init_dda_step(t_ray *ray, const t_player *player)
{
	init_dda_axis(player->pos.x, ray->map.x, ray->ray_dir.x,
		&ray->side_dist.x, &ray->delta_dist.x, &ray->step.x);
	init_dda_axis(player->pos.y, ray->map.y, ray->ray_dir.y,
		&ray->side_dist.y, &ray->delta_dist.y, &ray->step.y);
}
