#include <stdlib.h>
#include <stdint.h>
#include "libft.h"
#include "parse.h"

static int			count_lines(const char *s);
static void			free_partial_lines(char **lines, int count);
static t_system_err	fill_lines(char **result, const char *content);

/**
 * @brief 改行区切りの文字列を NULL 終端の行配列へ分割します。
 *
 * @param content 分割対象の入力文字列。内容は変更しません。
 * @param lines 確保済みの行配列を受け取る出力先。
 *
 * @retval SUCCESS 分割に成功した場合。
 * @retval READ_ERR 不正な引数が渡された場合。
 * @retval OVFL_ERR 行数が扱える範囲を超えた場合。
 * @retval MALLOC_ERR 行文字列の複製中にメモリ確保へ失敗した場合。
 */
t_system_err	split_lines(const char *content, char ***lines)
{
	char		**result;
	int			line_count;

	if (content == NULL || lines == NULL)
		return (READ_ERR);
	*lines = NULL;
	line_count = count_lines(content);
	if (line_count < 0)
		return (OVFL_ERR);
	result = malloc(sizeof(char *) * ((size_t)line_count + 1));
	if (result == NULL)
		return (malloc_err());
	if (fill_lines(result, content) != SUCCESS)
		return (MALLOC_ERR);
	*lines = result;
	return (SUCCESS);
}

static t_system_err	fill_lines(char **result, const char *content)
{
	const char	*start;
	const char	*end;
	int			i;

	i = 0;
	start = content;
	end = ft_strchr(start, '\n');
	while (end)
	{
		result[i] = ft_substr(start, 0, (size_t)(end - start));
		if (result[i] == NULL)
			return (free_partial_lines(result, i), malloc_err());
		i++;
		start = end + 1;
		end = ft_strchr(start, '\n');
	}
	result[i] = ft_strdup(start);
	if (result[i] == NULL)
		return (free_partial_lines(result, i), malloc_err());
	result[i + 1] = NULL;
	return (SUCCESS);
}

/**
 * @brief 改行区切り文字列に含まれる行数を数えます。
 *
 * @param s 入力文字列。
 *
 * @return 行数。入力不正またはオーバーフロー時は -1 を返します。
 */
static int	count_lines(const char *s)
{
	int	n;

	if (s == NULL)
		return (-1);
	n = 1;
	while (*s)
	{
		if (*s == '\n')
		{
			if (n == INT32_MAX)
				return (-1);
			n++;
		}
		s++;
	}
	return (n);
}

/**
 * @brief 生成途中の行配列を部分的に解放します。
 *
 * @param lines 構築途中の行配列。
 * @param count lines 内で初期化済みの要素数。
 */
static void	free_partial_lines(char **lines, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}
