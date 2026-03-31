#include "cub_config.h"

static char	row_cell_at(const char *row, int x)
{
	int	index;

	if (row == NULL || x < 0)
		return (' ');
	index = 0;
	while (index < x && row[index] != '\0')
		++index;
	if (index != x || row[index] == '\0')
		return (' ');
	return (row[index]);
}

char	map_cell_at(const t_map *map, int x, int y)
{
	if (map == NULL || y < 0 || y >= map->height)
		return (' ');
	return (row_cell_at(map->grid[y], x));
}

bool	map_is_solid(const t_map *map, int x, int y)
{
	char	cell;

	cell = map_cell_at(map, x, y);
	return (cell == '1' || cell == ' ');
}
