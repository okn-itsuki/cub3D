#include <stdlib.h>
#include "cub_config.h"
#include "parse.h"

static void free_tex_paths(t_tex_path *tex);
static void free_map_grid(t_map *map);

/**
 * @brief t_config が保持するヒープ領域をすべて解放する
 * @param config 解放対象の設定構造体
 */
void free_config(t_config *config)
{
	free_tex_paths(&config->tex);
	free_map_grid(&config->map);
}

/**
 * @brief テクスチャパス配列を解放する
 * @param tex 解放対象のテクスチャ情報
 */
static void free_tex_paths(t_tex_path *tex)
{
	int i;

	i = 0;
	while (i < TEX_COUNT)
	{
		free(tex->path[i]);
		tex->path[i] = NULL;
		i++;
	}
}

/**
 * @brief マップグリッドの各行と配列本体を解放する
 * @param map 解放対象のマップ情報
 */
static void free_map_grid(t_map *map)
{
	int i;

	if (map->grid == NULL)
		return;
	i = 0;
	while (i < map->height)
	{
		free(map->grid[i]);
		map->grid[i] = NULL;
		i++;
	}
	free(map->grid);
	map->grid = NULL;
}
