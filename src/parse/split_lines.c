#include <stdlib.h>
#include <stdint.h>
#include "libft.h"
#include "parse.h"

static int count_lines(const char *s);

/**
 * @brief 文字列を '\n' で分割し NULL 終端の行配列を返す
 * @param content 分割対象の文字列
 * @return 行配列 (NULL 終端)。失敗時は NULL
 * @return count_lines が失敗（オバーフロー）時に NULL
 * @note ft_split と異なり空行も配列に含める
 */
char **split_lines(char *content)
{
	char **lines;
	char *start;
	char *end;
	int i;
	int line_size;

	line_size = count_lines(content);
	if (line_size == -1)
		return (NULL);
	lines = malloc(sizeof(char *) * (line_size + 1));
	if (!lines)
		return (NULL);
	i = 0;
	start = content;
	end = ft_strchr(start, '\n');
	while (end)
	{
		lines[i++] = ft_substr(start, 0, end - start);
		start = end + 1;
		end = ft_strchr(start, '\n');
	}
	lines[i++] = ft_strdup(start);
	lines[i] = NULL;
	return (lines);
}

/**
 * @brief '\n' の出現回数から行数を数える (最低 1 行)
 * @param s カウント対象の文字列
 * @return 行数
 */
static int count_lines(const char *s)
{
	int n;

	n = 1;
	while (*s)
	{
		if (*s == '\n')
			n++;
		s++;
	}
	if (n == INT32_MAX)
		return (-1);
	return (n);
}
