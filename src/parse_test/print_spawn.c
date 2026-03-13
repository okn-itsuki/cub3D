#include "parse_test.h"
#include "cub_config.h"
#include <stdio.h>

static const char	*dir_to_str(t_dir dir)
{
	if (dir == NORTH)
		return ("NORTH");
	if (dir == SOUTH)
		return ("SOUTH");
	if (dir == EAST)
		return ("EAST");
	if (dir == WEST)
		return ("WEST");
	return ("UNKNOWN");
}

// 概要 : スポーン座標と初期方角を表示する
void	print_spawn(const t_spawn *spawn)
{
	printf("[spawn]  row=%d col=%d dir=%s\n",
		spawn->row, spawn->col, dir_to_str(spawn->dir));
}
