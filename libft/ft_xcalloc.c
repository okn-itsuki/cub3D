/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_xcalloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 02:27:48 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/05 02:35:58 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_xcalloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	total;

	if (size != 0 && count > SIZE_MAX / size)
	{
		ft_putstr_fd("fatal: allocation overflow\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	total = count * size;
	ptr = malloc(total);
	if (!ptr)
	{
		ft_putstr_fd("fatal: out of memory\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	ft_bzero(ptr, total);
	return (ptr);
}
