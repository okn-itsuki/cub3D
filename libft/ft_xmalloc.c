/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_xmalloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 23:45:06 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/05 02:35:56 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_xmalloc(size_t n)
{
	void	*ptr;

	ptr = malloc(n);
	if (!ptr)
	{
		ft_putstr_fd("malloc", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	return (ptr);
}
