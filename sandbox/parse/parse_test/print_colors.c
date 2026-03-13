#include "parse_test.h"
#include "cub_config.h"
#include <stdio.h>

static void	print_rgb(const char *label, const t_rgb *color)
{
	if (color->is_set)
		printf("[%s]  r=%d g=%d b=%d value=0x%06X is_set=true\n",
			label, color->r, color->g, color->b, color->value);
	else
		printf("[%s]  (unset)\n", label);
}

// 概要 : 床色・天井色 (F/C) を表示する
void	print_colors(const t_rgb *floor_color, const t_rgb *ceiling_color)
{
	print_rgb("floor_color", floor_color);
	print_rgb("ceiling_color", ceiling_color);
}
