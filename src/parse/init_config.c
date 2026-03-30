/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iokuno <iokuno@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 16:45:16 by iokuno            #+#    #+#             */
/*   Updated: 2026/03/14 21:34:53 by iokuno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file init_config.c
 * @brief t_configの初期化と破棄
 */
#include "libft.h"
#include "cub_config.h"
#include <stdlib.h>

static void	init_rgb(t_rgb *rgb);

/**
 * @brief t_configの全フィールドを未設定状態で初期化する
 *
 * テクスチャパスをNULL、色を未設定、マップをNULL/0、
 * スポーン位置を-1、方角をDIR_UNSETに設定する。
 *
 * @param[out] config 初期化する設定構造体
 */
void	init_config(t_config *config)
{
	int	i;

	i = 0;
	while (i < TEX_COUNT)
	{
		config->tex.path[i] = NULL;
		i++;
	}
	init_rgb(&config->floor_color);
	init_rgb(&config->ceiling_color);
	config->map.grid = NULL;
	config->map.width = 0;
	config->map.height = 0;
	config->spawn.row = -1;
	config->spawn.col = -1;
	config->spawn.dir = DIR_UNSET;
}

/**
 * @brief t_rgbの全フィールドをゼロ/未設定で初期化する
 *
 * @param[out] rgb 初期化するRGB構造体
 */
static void	init_rgb(t_rgb *rgb)
{
	rgb->r = 0;
	rgb->g = 0;
	rgb->b = 0;
	rgb->value = 0;
	rgb->is_set = false;
}

/**
 * @brief t_configが所有する動的メモリを解放する
 *
 * テクスチャパス配列とマップgridの各行・配列本体をfreeする。
 *
 * @param[in,out] config 解放する設定構造体 (NULLなら何もしない)
 */
void	destroy_config(t_config *config)
{
	int	i;

	if (config == NULL)
		return ;
	i = 0;
	while (i < TEX_COUNT)
	{
		free(config->tex.path[i]);
		config->tex.path[i] = NULL;
		i++;
	}
	if (config->map.grid == NULL)
		return ;
	i = 0;
	while (i < config->map.height)
	{
		free(config->map.grid[i]);
		i++;
	}
	free(config->map.grid);
	config->map.grid = NULL;
}
