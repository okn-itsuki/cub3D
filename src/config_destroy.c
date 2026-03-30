#include "game_init.h"
#include <stdlib.h>

static void	reset_rgb(t_rgb *rgb)
{
	rgb->r = 0;
	rgb->g = 0;
	rgb->b = 0;
	rgb->value = 0;
	rgb->is_set = false;
}

static void	reset_tex_paths(t_tex_path *tex)
{
	int	index;

	index = 0;
	while (index < TEX_COUNT)
	{
		tex->path[index] = NULL;
		++index;
	}
}

static void	reset_config_state(t_config *config)
{
	reset_tex_paths(&config->tex);
	reset_rgb(&config->floor_color);
	reset_rgb(&config->ceiling_color);
	config->map.grid = NULL;
	config->map.width = 0;
	config->map.height = 0;
	config->spawn.row = -1;
	config->spawn.col = -1;
	config->spawn.dir = DIR_UNSET;
}

static void	destroy_map(t_map *map)
{
	int	row;

	if (map->grid == NULL)
		return ;
	row = 0;
	while (row < map->height)
		free(map->grid[row++]);
	free(map->grid);
	map->grid = NULL;
}

void	destroy_config(t_config *config)
{
	int	index;

	if (config == NULL)
		return ;
	index = 0;
	while (index < TEX_COUNT)
	{
		free(config->tex.path[index]);
		config->tex.path[index] = NULL;
		++index;
	}
	destroy_map(&config->map);
	reset_config_state(config);
}
