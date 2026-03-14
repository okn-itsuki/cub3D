

#include "cub3d.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
parserを通さずに描画とraycastを実装するための簡易エントリポイント.
maps/good/library.cub 相当のt_configを手動で組み立てる.

F 139,99,58
C 171,154,137

NO texture/direction/north.xpm
SO texture/direction/south.xpm
WE texture/direction/west.xpm
EA texture/direction/east.xpm

1111111111111111111
1001001001001001001
1001001001001001001
1001001001001001001
1001001000001001001
100000000S000000001
1000011111111000001
1111000000000001111
1000000101010000001
1111111111111111111


*/


static void	free_sandbox_config(t_config *config){
	int	i;

	i = 0;
	while (i < TEX_COUNT){
		free(config->tex.path[i]);
		config->tex.path[i] = NULL;
		i++;
	}
	if (config->map.grid == NULL)
		return ;
	i = 0;
	while (i < config->map.height){
		free(config->map.grid[i]);
		i++;
	}
	free(config->map.grid);
	config->map.grid = NULL;
}

static bool	init_tex_path(t_tex_path *tex){
	tex->path[TEX_NO] = strdup("../../texture/direction/north.xpm");
	tex->path[TEX_SO] = strdup("../../texture/direction/south.xpm");
	tex->path[TEX_WE] = strdup("../../texture/direction/west.xpm");
	tex->path[TEX_EA] = strdup("../../texture/direction/east.xpm");
	return tex->path[TEX_NO] != NULL && tex->path[TEX_SO] != NULL
		&& tex->path[TEX_WE] != NULL && tex->path[TEX_EA] != NULL;
}

static inline void	add_value(t_rgb *color){
	color->value = (color->r << 16) | (color->g << 8) | color->b;
}

static void	init_color(t_rgb *floor_color, t_rgb *ceiling_color){
	floor_color->r = 139;
	floor_color->g = 99;
	floor_color->b = 58;
	floor_color->is_set = true;
	add_value(floor_color);
	ceiling_color->r = 171;
	ceiling_color->g = 154;
	ceiling_color->b = 137;
	ceiling_color->is_set = true;
	add_value(ceiling_color);
}

static const char	*g_map_src[] = {
	"1111111111111111111",
	"1001001001001001001",
	"1001001001001001001",
	"1001001001001001001",
	"1001001000001001001",
	"100000000S000000001",
	"1000011111111000001",
	"1111000000000001111",
	"1000000101010000001",
	"1111111111111111111"
};

static inline int	get_map_height(void){
	return (int)(sizeof(g_map_src) / sizeof(g_map_src[0]));
}

static int	get_map_width(void){
	int		i;
	int		width;
	size_t	row_len;

	i = 0;
	width = 0;
	while (i < get_map_height()){
		row_len = strlen(g_map_src[i]);
		if ((int)row_len > width)
			width = (int)row_len;
		i++;
	}
	return width;
}

static bool	get_spawn_dir(char cell, t_dir *dir)
{
	if (cell == 'N')
		*dir = NORTH;
	else if (cell == 'S')
		*dir = SOUTH;
	else if (cell == 'E')
		*dir = EAST;
	else if (cell == 'W')
		*dir = WEST;
	else
		return false;
	return true;
}

static bool	init_map_and_spawn(t_map *map, t_spawn *spawn){
	t_dir	dir;
	int		x;
	int		y;
	bool	found_spawn;

	map->height = get_map_height();
	map->width = get_map_width();
	map->grid = calloc((size_t)map->height + 1, sizeof(*map->grid));
	if (map->grid == NULL)
		return false;
	found_spawn = false;
	y = 0;
	while (y < map->height){
		map->grid[y] = strdup(g_map_src[y]);
		if (map->grid[y] == NULL)
			return false;
		x = 0;
		while (map->grid[y][x] != '\0'){
			if (get_spawn_dir(map->grid[y][x], &dir)){
				if (found_spawn)
					return false;
				found_spawn = true;
				spawn->row = y;
				spawn->col = x;
				spawn->dir = dir;
				map->grid[y][x] = '0';
			}
			x++;
		}
		y++;
	}
	return found_spawn;
}

static bool	init_sandbox_config(t_config *config){
	memset(config, 0, sizeof(*config));
	if (!init_tex_path(&config->tex)){
		free_sandbox_config(config);
		return false;
	}
	init_color(&config->floor_color, &config->ceiling_color);
	if (!init_map_and_spawn(&config->map, &config->spawn)){
		free_sandbox_config(config);
		return false;
	}
	return true;
}


static void print_texture_path(t_tex_path path_dir){
	printf("%s\n",path_dir.path[TEX_NO]);
	printf("%s\n",path_dir.path[TEX_SO]);
	printf("%s\n",path_dir.path[TEX_WE]);
	printf("%s\n",path_dir.path[TEX_EA]);
}

#include <stdio.h>

static void	debug_print_texture_image_one(const t_texture *tex)
{
	if (tex == NULL)
		return ;
	printf(
		"loaded=%d img=%p w=%d h=%d addr=%p bpp=%d line=%d endian=%d\n",
		(int)tex->loaded,
		(void *)tex->image.img,
		tex->image.width,
		tex->image.height,
		(void *)tex->image.addr,
		tex->image.bpp,
		tex->image.line_len,
		tex->image.endian
	);
}

void	debug_print_texture_image_all(const t_assets *assets)
{
	debug_print_texture_image_one(&assets->wall[TEX_NO]);
	debug_print_texture_image_one(&assets->wall[TEX_SO]);
	debug_print_texture_image_one(&assets->wall[TEX_WE]);
	debug_print_texture_image_one(&assets->wall[TEX_EA]);
}
#include "game_mlx_init.h"

bool	texture_xpm_load_all(t_assets *assets, t_tex_path path_dir, t_mlx mlx);

int	main(void)
{
	t_config	config;
	t_game		game;
	bool		ok;

	config = (t_config){0};
	game = (t_game){0};
	if (!init_sandbox_config(&config))
		return (1);
	init_game_mlx(&game);
	ok = texture_xpm_load_all(&game.assets, config.tex, game.mlx);
	debug_print_texture_image_all(&game.assets);
	print_texture_path(config.tex);
	free_sandbox_config(&config);
	if (!ok)
		return (1);
	return (0);
}