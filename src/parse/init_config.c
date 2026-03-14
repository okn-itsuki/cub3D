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

#include "libft.h"
#include "cub_config.h"

static void	init_rgb(t_rgb *rgb);

// 概要 : `t_config`の初期化
//
// - テクスチャパス配列をすべてNULLで初期化
// - 床色(`floor_color`)を未設定状態で初期化
// - 天井色（`floor_color`）を未設定状態で初期化
// - `map`本体のポインタを未設定(NULL)にする
// - `map`の幅を 0 で初期化する
// - `map`の高さを 0 で初期化する
// - `spawn`行位置を未設定として -1 にする
// - `spawn`列位置を未設定として -1 にする
// - 方角の暫定値をNとする

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

// 概要 : t_rgbの初期化
static void	init_rgb(t_rgb *rgb)
{
	rgb->r = 0;
	rgb->g = 0;
	rgb->b = 0;
	rgb->value = 0;
	rgb->is_set = false;
}
