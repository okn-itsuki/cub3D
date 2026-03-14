/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_xopen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iokuno <iokuno@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:05:27 by iokuno            #+#    #+#             */
/*   Updated: 2026/03/14 14:44:56 by iokuno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <fcntl.h>
#include <stdio.h>

// 概要 : ファイルを読み取り専用でopenしてfdを返す
// メモリ開放義務 : fdを使用後にclose
// エラー時の処理 : 正常にopenできなければ、エラー出力後にプログラムをエラー終了する
int	ft_xopen(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	return (fd);
}
