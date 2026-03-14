/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_extension.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iokuno <iokuno@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 12:09:07 by iokuno            #+#    #+#             */
/*   Updated: 2026/03/14 12:39:11 by iokuno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "libft.h"

// 概要 : CLI引数の拡張子が指定されたものと一致するかの確認。
// 戻り地 :
// - false : 失敗
// - true : 成功
bool	has_extension(const char *filename, const char *extension)
{
	const char *dot_pos;

	dot_pos = ft_strrchr(filename, '.');
	if (dot_pos == NULL)
		return (false);
	if (ft_strcmp(dot_pos, extension) == 0)
		return (true);
	return (false);
}
