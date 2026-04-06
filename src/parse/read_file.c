#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "parse.h"

#define BUF_SIZE 4096

static t_system_err	read_fd(int fd, char **content);
static t_system_err	append_buf(char **content, char *buf, ssize_t read_size);
static t_system_err	open_file(const char *path, int *fd);

t_system_err	read_file_lines(const char *path, char ***ptr)
{
	int				fd;
	char			*content;
	char			**lines;
	t_system_err	state;

	if (path == NULL || ptr == NULL)
		return (READ_ERR);
	*ptr = NULL;
	fd = -1;
	content = NULL;
	lines = NULL;
	state = open_file(path, &fd);
	if (state != SUCCESS)
		return (state);
	state = read_fd(fd, &content);
	if (close(fd) < 0 && state == SUCCESS)
		state = CLOSE_ERR;
	if (state != SUCCESS)
		return (free(content), state);
	state = split_lines(content, &lines);
	free(content);
	if (state != SUCCESS)
		return (state);
	*ptr = lines;
	return (SUCCESS);
}

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

static t_system_err	open_file(const char *path, int *fd)
{
	*fd = open(path, O_RDONLY);
	if (*fd < 0)
	{
		ft_putstr_fd("Error\n", STDERR_FILENO);
		ft_putstr_fd("failed to open file\n", STDERR_FILENO);
		return (OPEN_ERR);
	}
	return (SUCCESS);
}

static t_system_err	append_buf(char **content, char *buf, ssize_t read_size)
{
	char	*tmp;

	buf[read_size] = '\0';
	tmp = ft_strjoin(*content, buf);
	if (tmp == NULL)
		return (MALLOC_ERR);
	free(*content);
	*content = tmp;
	return (SUCCESS);
}

static t_system_err	read_fd(int fd, char **content)
{
	char			buf[BUF_SIZE + 1];
	ssize_t			read_size;
	t_system_err	state;

	*content = ft_strdup("");
	if (*content == NULL)
		return (MALLOC_ERR);
	read_size = read(fd, buf, BUF_SIZE);
	while (read_size > 0)
	{
		state = append_buf(content, buf, read_size);
		if (state != SUCCESS)
			return (state);
		read_size = read(fd, buf, BUF_SIZE);
	}
	if (read_size < 0)
	{
		free(*content);
		*content = NULL;
		return (READ_ERR);
	}
	return (SUCCESS);
}
