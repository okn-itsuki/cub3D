#include <stdlib.h>
#include <stdint.h>
#include "libft.h"
#include "parse.h"

static int	count_lines(const char *s);
static void	free_partial_lines(char **lines, int count);

t_system_err	split_lines(char *content, char ***lines)
{
	char	**result;
	char	*start;
	char	*end;
	int		i;
	int		line_count;

	if (content == NULL || lines == NULL)
		return (READ_ERR);
	*lines = NULL;
	line_count = count_lines(content);
	if (line_count < 0)
		return (OVFL_ERR);
	result = malloc(sizeof(char *) * ((size_t)line_count + 1));
	if (result == NULL)
		return (MALLOC_ERR);
	i = 0;
	start = content;
	end = ft_strchr(start, '\n');
	while (end)
	{
		result[i] = ft_substr(start, 0, (size_t)(end - start));
		if (result[i] == NULL)
			return (free_partial_lines(result, i), MALLOC_ERR);
		i++;
		start = end + 1;
		end = ft_strchr(start, '\n');
	}
	result[i] = ft_strdup(start);
	if (result[i] == NULL)
		return (free_partial_lines(result, i), MALLOC_ERR);
	i++;
	result[i] = NULL;
	*lines = result;
	return (SUCCESS);
}

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
