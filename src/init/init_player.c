#include "game_init.h"
#include "game_config.h"

static t_vec2d	get_dir_vector(t_dir dir)
{
	static const t_vec2d	table[] = {
		[EAST] = {1.0, 0.0},
		[WEST] = {-1.0, 0.0},
		[NORTH] = {0.0, -1.0},
		[SOUTH] = {0.0, 1.0}
	};

	if (dir < EAST || dir > SOUTH)
		return ((t_vec2d){0.0, 0.0});
	return (table[dir]);
}

static t_vec2d	get_plane_vector(t_dir dir)
{
	static const t_vec2d	table[] = {
		[EAST] = {0.0, FOV_HALF_TAN},
		[WEST] = {0.0, -FOV_HALF_TAN},
		[NORTH] = {FOV_HALF_TAN, 0.0},
		[SOUTH] = {-FOV_HALF_TAN, 0.0}
	};

	if (dir < EAST || dir > SOUTH)
		return ((t_vec2d){0.0, 0.0});
	return (table[dir]);
}

void	init_player(t_player *player, t_spawn spawn)
{
	if (player == NULL)
		return ;
	player->pos.x = spawn.col + 0.5;
	player->pos.y = spawn.row + 0.5;
	player->dir = get_dir_vector(spawn.dir);
	player->plane = get_plane_vector(spawn.dir);
	player->move_speed = PLAYER_MOVE_SPEED;
	player->rot_speed = PLAYER_ROT_SPEED;
}
