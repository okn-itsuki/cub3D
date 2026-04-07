#include "cub3d.h"
#include <stdio.h>

static void	debug_print_vec2d(const char *label, t_vec2d v)
{
	printf("  %s = (%f, %f)\n", label, v.x, v.y);
}

void	debug_print_player_pos_all(const t_player player)
{
	printf("player: \n");
	debug_print_vec2d("- pos", player.pos);
	debug_print_vec2d("- dir", player.dir);
	debug_print_vec2d("- plane", player.plane);
	printf("  - move_speed = %f\n", player.move_speed);
	printf("  - rot_speed  = %f\n", player.rot_speed);
}
