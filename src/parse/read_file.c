
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "parse.h"

#define BUF_SIZE 4096

static t_system_err read_fd(int fd, char *res);
static t_system_err append_buf(char *content, char *buf, ssize_t n);
static t_system_err open_file(const char *path, int *fd);
static t_system_err state_file_lines(t_system_err state);

/**
 * @brief .cub ファイルを行単位で読み込み NULL 終端の配列を返す
 * @param path 読み込むファイルパス
 * @return 行配列 (NULL 終端)。失敗時は NULL
 */
t_system_err read_file_lines(const char *path, char ***ptr)
{
	int fd;
	char *content;
	char **lines;
	t_system_err state;

	state = open_file(path, &fd);
	if (state != SUCCESS)
		return (state_file_lines(state));
	state = read_fd(fd, content);
	close(fd);
	if (content == NULL)
		return (state_file_lines(state));
	state = split_lines(content, lines);
	if (state != SUCCESS)
		return (state_file_lines(state));
	free(content);
	*ptr = lines;
	return (SUCCESS);
}

static t_system_err state_file_lines(t_system_err state)
{
	if (state == OVFL_ERR)
		return (OVFL_ERR);
	if (state == OPEN_ERR)
		return (OPEN_ERR);
	if (state == READ_ERR)
		return (READ_ERR);
	if (state == MALLOC_ERR)
		return (MALLOC_ERR);
	if (state == SUCCESS)
		return (SUCCESS);
};

/**
 * @brief read_file_lines の戻り値を解放する
 * @param lines 解放対象の行配列
 */
void free_lines(char **lines)
{
	int i;

	if (!lines)
		return;
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
static t_system_err open_file(const char *path, int *fd)
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

/**
 * @brief chunk を既存の content に結合して返す。content は解放される
 * @return 新しい結合済み文字列。失敗時 NULL
 */
static t_system_err append_buf(char *content, char *buf, ssize_t n)
{
	char *tmp;

	buf[n] = '\0';
	tmp = ft_strjoin(content, buf);
	if (tmp == NULL)
		return (MALLOC_ERR);
	free(content);
	content = tmp;
	return (SUCCESS);
}

/**
 * @brief ファイル記述子の全内容をヒープ文字列として読み込む
 * @param fd 読み込むファイル記述子
 * @return ヒープ上の文字列。失敗時 NULL
 */
static t_system_err read_fd(int fd, char *content)
{
	char buf[BUF_SIZE + 1];
	char *tmp;
	ssize_t n;
	t_system_err state;

	tmp = ft_strdup("");
	if (!tmp)
		return (MALLOC_ERR);
	n = read(fd, buf, BUF_SIZE);
	while (n > 0)
	{
		state = append_buf(tmp, buf, n);
		if (state != SUCCESS)
			return (state_file_lines(state));
		n = read(fd, buf, BUF_SIZE);
	}
	if (n < 0)
	{
		free(tmp);
		return (READ_ERR);
	}
	content = tmp;
	return (SUCCESS);
}
