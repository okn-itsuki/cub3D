/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_all_lines.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iokuno <iokuno@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:26:54 by iokuno            #+#    #+#             */
/*   Updated: 2026/03/14 16:39:35 by iokuno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>

// 概要 : ファイルの中身をメモリに書き込む
// 返り値 : ファイルを1行ずつ保有する二次元配列（ダブルポインタ）
// エラー処理 :
const char	**read_all_lines(int fd)
{
	const char	**line;
	int			i;

	i = 0;
	while (true)
	{
		line[i] = get_next_line(fd);
		if (line[i] == NULL)
			break ;
		i++;
	}
// TODO : issue #5
	return (line);
}
