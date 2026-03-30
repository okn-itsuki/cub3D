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
	return (map_is_solid(map, (int)x, (int)y));
}

static void	add_scaled_vector(t_vec2d *dst, t_vec2d axis, double scale)
{
	dst->x += axis.x * scale;
	dst->y += axis.y * scale;
}

static t_vec2d	get_strafe_axis(const t_player *player)
{
	return ((t_vec2d){
		player->plane.x / FOV_HALF_TAN,
		player->plane.y / FOV_HALF_TAN
	});
}

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
