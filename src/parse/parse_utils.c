/**
 * @file parse_utils.c
 * @brief parse モジュール内で共有する文字列補助関数
 */
#include <unistd.h>
#include "libft.h"
#include "parse.h"

/**
 * @brief parse 用のエラーメッセージを標準エラーへ出力します。
 *
 * @param message `Error` の次行に出力する説明文。NULL の場合は見出しだけ出力。
 *
 * @retval false 常に false を返します。
 */
bool parse_put_error(const char *message)
{
	ft_putstr_fd("Error\n", STDERR_FILENO);
	if (message != NULL)
	{
		ft_putstr_fd((char *)message, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	return (false);
}

/**
 * @brief 文字列先頭の空白文字を読み飛ばす。
 *
 * @param str 対象の文字列。
 *
 * @return 先頭の空白を読み飛ばした位置へのポインタ。
 */
const char *parse_skip_spaces(const char *str)
{
	while (*str != '\0' && ft_isspace((unsigned char)*str))
		str++;
	return (str);
}

/**
 * @brief 行が空行または空白のみか判定。
 *
 * @param line 判定対象の行文字列。
 *
 * @retval true 空行または NULL の場合。
 * @retval false 可視文字を含む場合。
 */
bool parse_is_blank_line(const char *line)
{
	if (line == NULL)
		return (true);
	line = parse_skip_spaces(line);
	return (*line == '\0');
}

/**
 * @brief 文字列の末尾に対象の文字列があるか
 *
 * @param filename 判定対象の文字列。
 * @param extension 拡張子の文字列。
 *
 * @retval true `filename` が `extension` で終わる場合。
 * @retval false 引数不正または接尾が一致しない場合。
 */
bool parse_extension(const char *filename, const char *extension)
{
	size_t filename_len;
	size_t extension_len;
	char *ex_position;

	if (filename == NULL || extension == NULL)
		return (false);
	filename_len = ft_strlen(filename);
	extension_len = ft_strlen(extension);
	ex_position = (char *)filename + (filename_len - extension_len);
	if (filename_len < extension_len)
		return (false);
	return (ft_strncmp((const char *)ex_position, extension, extension_len) == 0);
}

/**
 * @brief 1 行がヘッダ要素・マップ・空行のどれに属するか分類します。
 *
 * @param line 判定対象の行文字列。
 *
 * @return `t_parse_line_type` のいずれか。
 */
t_parse_line_type parse_get_line_type(const char *line)
{
	line = parse_skip_spaces(line);
	if (*line == '\0')
		return (LINE_EMPTY);
	if (ft_strncmp(line, "NO", 2) == 0 && ft_isspace((unsigned char)line[2]))
		return (LINE_TEXTURE);
	if (ft_strncmp(line, "SO", 2) == 0 && ft_isspace((unsigned char)line[2]))
		return (LINE_TEXTURE);
	if (ft_strncmp(line, "WE", 2) == 0 && ft_isspace((unsigned char)line[2]))
		return (LINE_TEXTURE);
	if (ft_strncmp(line, "EA", 2) == 0 && ft_isspace((unsigned char)line[2]))
		return (LINE_TEXTURE);
	if (*line == 'F' && ft_isspace((unsigned char)line[1]))
		return (LINE_COLOR);
	if (*line == 'C' && ft_isspace((unsigned char)line[1]))
		return (LINE_COLOR);
	if (*line == ' ' || *line == '0' || *line == '1')
		return (LINE_MAP);
	if (*line == 'N' || *line == 'S' || *line == 'E' || *line == 'W')
		return (LINE_MAP);
	return (LINE_INVALID);
}
