/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 04:42:17 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/12 13:38:33 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	len;
	char	*copy;
	char	*head;

	len = ft_strlen(s1);
	copy = (char *)malloc(len + 1);
	head = copy;
	if (!copy)
		return (NULL);
	while (*s1)
		*copy++ = *s1++;
	*copy = '\0';
	return (head);
}
