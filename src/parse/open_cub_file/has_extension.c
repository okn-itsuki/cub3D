/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_extension.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iokuno <iokuno@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 12:09:07 by iokuno            #+#    #+#             */
/*   Updated: 2026/03/14 14:51:10 by iokuno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>

// 概要 : CLI引数の拡張子が指定されたものと一致するかの確認。
// エラー処理 : filenameがNULL/'.'がない場合もfalseは返す。
// 戻り地 :
// - false : 失敗
// - true : 成功
bool	has_extension(const char *filename, const char *extension)
{
	const char	*dot_pos;

	dot_pos = ft_strrchr(filename, '.');
	if (dot_pos == NULL)
		return (false);
	if (ft_strcmp(dot_pos, extension) == 0)
		return (true);
	return (false);
}
