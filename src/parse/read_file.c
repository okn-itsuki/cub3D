#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "parse.h"

#define BUF_SIZE 4096

static t_system_err	read_fd(int fd, char **content);
static t_system_err	append_buf(char **content, char *buf, ssize_t read_size);

/**
 * @brief ファイルを読み込み、NULL 終端の行配列へ分割します。
 *
 * @param path 読み込むファイルのパス。
 * @param ptr 成功時に確保済みの行配列を受け取る出力先。
 *
 * @retval SUCCESS ファイルの読み込みと分割に成功した場合。
 * @retval OPEN_ERR ファイルのオープンに失敗した場合。
 * @retval READ_ERR 引数が不正、または読み込みに失敗した場合。
 * @retval CLOSE_ERR ファイルディスクリプタのクローズに失敗した場合。
 * @retval MALLOC_ERR 読み込みまたは分割中のメモリ確保に失敗した場合。
 * @retval OVFL_ERR 行数が扱える範囲を超えた場合。
 */
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
	if (close_file(fd) != SUCCESS && state == SUCCESS)
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

/**
 * @brief NULL 終端の文字列配列を解放します。
 *
 * @param lines read_file_lines() または split_lines() が確保した行配列。
 */
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

/**
 * @brief 読み込んだバッファを累積済みの文字列末尾に連結します。
 *
 * @param content 伸長対象のヒープ文字列。
 * @param buf 今回読み込んだ内容を持つバッファ。
 * @param read_size buf 内で有効なバイト数。
 *
 * @retval SUCCESS 文字列の連結に成功した場合。
 * @retval MALLOC_ERR 文字列連結時のメモリ確保に失敗した場合。
 */
static t_system_err	append_buf(char **content, char *buf, ssize_t read_size)
{
	char	*tmp;

	buf[read_size] = '\0';
	tmp = ft_strjoin(*content, buf);
	if (tmp == NULL)
		return (malloc_err());
	free(*content);
	*content = tmp;
	return (SUCCESS);
}

/**
 * @brief 開かれているファイルディスクリプタから全文を 1 つの文字列として読み込みます。
 *
 * @param fd オープン済みのファイルディスクリプタ。
 * @param content 確保済みのファイル内容文字列を受け取る出力先。
 *
 * @retval SUCCESS ファイル内容の読み込みに成功した場合。
 * @retval READ_ERR read() に失敗した場合。
 * @retval MALLOC_ERR 内容構築中のメモリ確保に失敗した場合。
 */
static t_system_err	read_fd(int fd, char **content)
{
	char			buf[BUF_SIZE + 1];
	ssize_t			read_size;
	t_system_err	state;

	*content = ft_strdup("");
	if (*content == NULL)
		return (malloc_err());
	state = read_file_buf(fd, buf, BUF_SIZE, &read_size);
	if (state != SUCCESS)
	{
		free(*content);
		*content = NULL;
		return (state);
	}
	while (read_size > 0)
	{
		state = append_buf(content, buf, read_size);
		if (state != SUCCESS)
			return (state);
		state = read_file_buf(fd, buf, BUF_SIZE, &read_size);
		if (state != SUCCESS)
		{
			free(*content);
			*content = NULL;
			return (state);
		}
	}
	return (SUCCESS);
}
