#include <stdio.h>
#include "config.h"

void	debug_cout_rgb(t_rgb *rgb)
{
	printf("  r      : %d\n", rgb->r);
	printf("  g      : %d\n", rgb->g);
	printf("  b      : %d\n", rgb->b);
	printf("  value  : 0x%06X\n", rgb->value);
	printf("  is_set : %s\n", rgb->is_set ? "true" : "false");
}

void	debug_cout_tex_path(t_tex_path *tex)
{
	printf("[TEX_PATH]\n");
	printf("  NO : %s\n", tex->no ? tex->no : "(null)");
	printf("  SO : %s\n", tex->so ? tex->so : "(null)");
	printf("  WE : %s\n", tex->we ? tex->we : "(null)");
	printf("  EA : %s\n", tex->ea ? tex->ea : "(null)");
}

void	debug_cout_spawn(t_spawn *spawn)
{
	printf("[SPAWN]\n");
	printf("  row   : %d\n", spawn->row);
	printf("  col   : %d\n", spawn->col);
	printf("  dir   : %c\n", spawn->dir);
	printf("  count : %d\n", spawn->count);
}

void	debug_cout_map(t_map *map)
{
	int	i;

	printf("[MAP]\n");
	printf("  width  : %d\n", map->width);
	printf("  height : %d\n", map->height);
	printf("  grid   :\n");
	i = 0;
	while (i < map->height)
	{
		printf("    [%d] %s\n", i, map->grid[i] ? map->grid[i] : "(null)");
		i++;
	}
}

void	debug_cout_config(t_config *config)
{
	printf("=== CONFIG DEBUG ===\n");
	debug_cout_tex_path(&config->tex);
	printf("[FLOOR]\n");
	debug_cout_rgb(&config->floor);
	printf("[CEILING]\n");
	debug_cout_rgb(&config->ceiling);
	debug_cout_map(&config->map);
	debug_cout_spawn(&config->spawn);
	printf("====================\n");
}
