#include "parse_test.h"
#include "cub_config.h"
#include <stdio.h>

// 概要 : マップグリッドと寸法を表示する
void	print_map(const t_map *map)
{
	int	y;

	printf("[map]  width=%d height=%d\n", map->width, map->height);
	if (map->grid == NULL)
	{
		printf("  (null)\n");
		return ;
	}
	y = 0;
	while (y < map->height)
	{
		if (map->grid[y])
			printf("  %s\n", map->grid[y]);
		else
			printf("  (null row %d)\n", y);
		y++;
	}
}
