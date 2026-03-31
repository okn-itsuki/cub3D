/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iokuno <iokuno@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 00:00:00 by iokuno            #+#    #+#             */
/*   Updated: 2026/03/31 00:00:00 by iokuno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "cub_config.h"
#include "parse.h"

static void	free_tex_paths(t_tex_path *tex);
static void	free_map_grid(t_map *map);

// 概要 : t_config が保持するヒープ領域をすべて解放する
// 参照で変更された値 :
//   config->tex.path[] を NULL にする
//   config->map.grid を NULL にする
// メモリ開放義務 : parse_cub 成功後、呼び出し元が責任を持って呼ぶ
void	free_config(t_config *config)
{
	free_tex_paths(&config->tex);
	free_map_grid(&config->map);
}

// 概要 : テクスチャパス配列を解放する
static void	free_tex_paths(t_tex_path *tex)
{
	int	i;

	i = 0;
	while (i < TEX_COUNT)
	{
		free(tex->path[i]);
		tex->path[i] = NULL;
		i++;
	}
}

// 概要 : マップグリッドの各行と配列本体を解放する
static void	free_map_grid(t_map *map)
{
	int	i;

	if (map->grid == NULL)
		return ;
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
