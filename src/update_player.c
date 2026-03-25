#include "ray_casting.h"
#include "game_config.h"
#include <math.h>

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
	if (!is_wall(map, new_x + margin_x, player->pos.y))
		player->pos.x = new_x;
	if (!is_wall(map, player->pos.x, new_y + margin_y))
		player->pos.y = new_y;
}

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
