/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_all_lines.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iokuno <iokuno@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:26:54 by iokuno            #+#    #+#             */
/*   Updated: 2026/03/14 14:44:55 by iokuno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>

char	**read_all_lines(int fd)
{
	char	**lines;
	char	*next_line;
	char	**new_lines;
	size_t	capacity;
	size_t	count;

	capacity = 16;
	count = 0;
	lines = ft_xmalloc(sizeof(char *) * (capacity + 1));
	while (true)
	{
		next_line = get_next_line(fd);
		if (next_line == NULL)
			break;
		if (count == capacity)
		{
			capacity *= 2;
			new_lines = ft_xmalloc(sizeof(char *) * (capacity + 1));
			ft_memcpy(new_lines, lines, sizeof(char *) * count);
			free(lines);
			lines = new_lines;
		}
		lines[count] = next_line;
		count++;
	}
	lines[count] = NULL;
	return (lines);
}
