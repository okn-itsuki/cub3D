/**
 * @file config_destroy.c
 * @brief `t_config`が所有する動的メモリの解放
 */
#include "game_init.h"
#include <stdlib.h>

/**
 * @brief RGB構造体を未設定状態へ戻す
 *
 * @param[out] rgb リセット対象
 */
static void	reset_rgb(t_rgb *rgb)
{
	rgb->r = 0;
	rgb->g = 0;
	rgb->b = 0;
	rgb->value = 0;
	rgb->is_set = false;
}

/**
 * @brief テクスチャパス配列をすべて`NULL`へ戻す
 *
 * @param[out] tex リセット対象
 */
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

/**
 * @brief `t_config`の全フィールドを未設定状態へ戻す
 *
 * @param[out] config リセット対象
 */
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

/**
 * @brief マップの各行と行配列本体を解放する
 *
 * @param[in,out] map 解放対象
 */
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

/**
 * @brief `t_config`が所有する動的メモリを解放する
 *
 * テクスチャパス配列とマップgridをfreeした後,未設定状態へ戻す.
 *
 * @param[in,out] config 解放対象の設定構造体
 */
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
