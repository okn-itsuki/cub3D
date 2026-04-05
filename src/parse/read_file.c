/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iokuno <iokuno@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 00:00:00 by iokuno            #+#    #+#             */
/*   Updated: 2026/04/03 00:00:00 by iokuno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "parse.h"

#define BUF_SIZE 4096

static int		open_file(const char *path);
static char		*read_fd(int fd);
static char		*append_buf(char *content, char *buf, ssize_t n);

/**
 * @brief .cub ファイルを行単位で読み込み NULL 終端の配列を返す
 * @param path 読み込むファイルパス
 * @return 行配列 (NULL 終端)。失敗時は NULL
 */
char	**read_file_lines(const char *path)
{
	int		fd;
	char	*content;
	char	**lines;

	fd = open_file(path);
	if (fd < 0)
		return (NULL);
	content = read_fd(fd);
	close(fd);
	if (!content)
		return (NULL);
	lines = split_lines(content);
	free(content);
	return (lines);
}

/**
 * @brief read_file_lines の戻り値を解放する
 * @param lines 解放対象の行配列
 */
void	free_lines(char **lines)
{
	int	i;

	if (!lines)
		return ;
	i = 0;
	while (lines[i])
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

/**
 * @brief ファイルを O_RDONLY で開く。失敗時は stderr へ出力して -1 を返す
 */
static int	open_file(const char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("Error\n", STDERR_FILENO);
		ft_putstr_fd("failed to open file\n", STDERR_FILENO);
	}
	return (fd);
}

/**
 * @brief chunk を既存の content に結合して返す。content は解放される
 * @return 新しい結合済み文字列。失敗時 NULL
 */
static char	*append_buf(char *content, char *buf, ssize_t n)
{
	char	*tmp;

	buf[n] = '\0';
	tmp = ft_strjoin(content, buf);
	free(content);
	return (tmp);
}

/**
 * @brief ファイル記述子の全内容をヒープ文字列として読み込む
 * @param fd 読み込むファイル記述子
 * @return ヒープ上の文字列。失敗時 NULL
 */
static char	*read_fd(int fd)
{
	char	buf[BUF_SIZE + 1];
	char	*content;
	ssize_t	n;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	n = read(fd, buf, BUF_SIZE);
	while (n > 0)
	{
		content = append_buf(content, buf, n);
		if (!content)
			return (NULL);
		n = read(fd, buf, BUF_SIZE);
	}
	if (n < 0)
	{
		free(content);
		return (NULL);
	}
	return (content);
}

