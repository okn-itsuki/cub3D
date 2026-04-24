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

/*
** @brief 床色または天井色 1 件を表示する
** @param label 表示名
** @param color 出力する RGB 情報
**
** color->is_set が true のときは RGB 各成分と packed value を表示し、
** false のときは未設定として表示する。
*/

/*
** @brief 床色と天井色をまとめて表示する
** @param floor_color 床色 (F)
** @param ceiling_color 天井色 (C)
**
** それぞれを print_rgb() 経由で 1 行ずつ表示する。
*/
void	print_colors(const t_rgb *floor_color, const t_rgb *ceiling_color)
{
	print_rgb("floor_color", floor_color);
	print_rgb("ceiling_color", ceiling_color);
}
