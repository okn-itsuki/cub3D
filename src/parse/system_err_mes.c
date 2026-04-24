#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "libft.h"
#include "parse.h"

t_excepion	open_file(const char *path, int *fd)
{
	*fd = open(path, O_RDONLY);
	if (*fd < 0)
	{
		ft_putstr_fd("Error\n", STDERR_FILENO);
		perror("open");
		return (OPEN_ERR);
	}
	return (SUCCESS);
}

t_excepion	read_file_buf(int fd,
	char *buf, size_t buf_size, ssize_t *read_size)
{
	*read_size = read(fd, buf, buf_size);
	if (*read_size < 0)
	{
		ft_putstr_fd("Error\n", STDERR_FILENO);
		perror("read");
		return (READ_ERR);
	}
	return (SUCCESS);
}

t_excepion	close_file(int fd)
{
	if (close(fd) < 0)
	{
		ft_putstr_fd("Error\n", STDERR_FILENO);
		perror("close");
		return (CLOSE_ERR);
	}
	return (SUCCESS);
}

t_excepion	malloc_err(void)
{
	ft_putstr_fd("Error\n", STDERR_FILENO);
	ft_putstr_fd("memory allocation failed\n", STDERR_FILENO);
	return (MALLOC_FAIL);
}

t_excepion	excepion_map(const char *message)
{
	ft_putstr_fd("Error\n", STDERR_FILENO);
	ft_putstr_fd((char *)message, STDERR_FILENO);
	return (MAP_ERR);
// TODO add MAP_ERR
}

t_excepion excepion_argment(const char *message)
{
	ft_putstr_fd((char *)message,STDERR_FILENO);
	return (INVALID_STRING);
}

t_excepion excepion_texture(const char *message)
{
	ft_putstr_fd((char *)message,STDERR_FILENO);
	return (TEX_ERR);
}

t_excepion excepion_color(const char *message)
{
	ft_putstr_fd((char *)message,STDERR_FILENO);
	return (COLOR_ERR);
}

t_excepion excepion_message(const char *message, t_excepion state)
{
	ft_putstr_fd((char *)message, STDERR_FILENO);
	return (state);
}
