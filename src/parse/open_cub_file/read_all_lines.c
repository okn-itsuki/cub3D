/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_all_lines.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okunoitsuki <okunoitsuki@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:26:54 by iokuno            #+#    #+#             */
/*   Updated: 2026/03/22 21:42:58 by okunoitsuki      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

// 概要 : 配列を拡張する（realloc的な動き）
// 返り値 : NULL終端でsizeの大きさに拡張された配列
static char **expand_lines(char **lines, int size)
{
	char **new_lines;
	int i;

	new_lines = malloc(sizeof(char *) * (size + 1));
	if (!new_lines)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_lines[i] = lines[i];
		i++;
	}
	new_lines[size] = NULL;
	free(lines);
	return (new_lines);
}

// 概要 : linesを全解放
static void free_lines(char **lines)
{
	int i;

	if (!lines)
		return;
	i = 0;
	while (lines[i])
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

// 概要 : FDから全行をメモリに書き込みNULL終端のchar**で返す
// 返り値 : ファイルを1行ずつ保有する二次元配列（ダブルポインタ）
// エラー処理 :
// 開放義務 : アリ

char **read_all_lines(int fd)
{
	char *line;
	char **lines;
	int count;

	lines = NULL;
	count = 0;
	while (1)
	{
		// TODO : issue 5
		line = get_next_line(fd);
		if (!line)
			break;
		lines = expand_lines(lines, count);
		if (!lines)
		{
			free(line);
			free_lines(lines);
			return (NULL);
		}
		lines[count] = line;
		count++;
	}
	lines = expand_lines(lines, count);
	if (!lines)
		return (NULL);
	return (lines);
}
