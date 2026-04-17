/**
 * @file parse_texture.c
 * @brief テクスチャ指定行の解析
 */
#include <stdlib.h>
#include "libft.h"
#include "parse.h"

static int			get_tex_id(const char *line);
static t_excepion	set_texture_path(const char *line, int tex_id, t_config *config);
static t_excepion	dup_texture_path(const char *line);
static t_excepion	is_valid_texture_path(char *path);

/**
 * @brief テクスチャ指定行を解析して
 * 対応するパスを保存します。
 *
 * @param line `NO` `SO` `WE` `EA` のいずれかで始まる行。
 * @param config テクスチャパスの保存先。
 *
 * @retval true テクスチャ識別子と `.xpm` パスを
 * 正しく解析できた場合。
 * @retval false 重複、欠落、不正パス、
 * またはメモリ確保失敗時。
 */
t_excepion	parse_texture_line(const char *line, t_config *config)
{
	int	tex_id;

	line = parse_skip_spaces(line);
	tex_id = get_tex_id(line);
// TODO : issue33 例外の種類を増やす？
	if (tex_id < 0)
		return (excepiom_argment("unknown texture identifier"));
	if (config->tex.path[tex_id] != NULL)
		return (excepiom_argment("duplicate texture identifier"))
	return (set_texture_path(line, tex_id, config));
}

/**
 * @brief テクスチャ識別子を `t_tex_id` へ変換します。
 *
 * @param line `NO` `SO` `WE` `EA` のいずれかを先頭に持つ行。
 *
 * @return 対応する `t_tex_id`。識別できない場合は `-1`。
 */
static int	get_tex_id(const char *line)
{
	if (ft_strncmp(line, "NO", 2) == 0)
		return (TEX_NO);
	if (ft_strncmp(line, "SO", 2) == 0)
		return (TEX_SO);
	if (ft_strncmp(line, "WE", 2) == 0)
		return (TEX_WE);
	if (ft_strncmp(line, "EA", 2) == 0)
		return (TEX_EA);
	return (-1);
}

/**
 * @brief 識別子の後続からテクスチャパスを取り出して保存します。
 *
 * @param line テクスチャ識別子を含む入力行。
 * @param tex_id 保存先テクスチャスロットの index。
 * @param config テクスチャパスの保存先。
 *
 * @retval true パスの複製と妥当性検証に成功した場合。
 * @retval false パス欠落、妥当性不正、確保失敗があった場合。
 */
static t_excepion	set_texture_path(const char *line, int tex_id, t_config *config)
{
	char	*path;
	t_excepion state;

	state = dup_texture_path(parse_skip_spaces(path, line + TEX_ID_LEN));
	if (state != SUCCESS)
		return (state);
	state = is_valid_texture_path(path);
	if (state != SUCCESS)
	{
		free(path);
		return (state);
	}
	config->tex.path[tex_id] = path;
	return (SUCCESS);
}

/**
 * @brief 行末空白を除いたテクスチャパス文字列を複製します。
 *
 * @param line 識別子と空白を読み飛ばした後のパス文字列。
 *
 * @return 複製したパス文字列。パスが空、または確保失敗時は `NULL`。
 */
static t_excepion dup_texture_path(char *path, const char *line)
{
	size_t		len;
	char		*res;
	t_excepion	state;

	len = ft_strlen(line);
	while (len > 0 && ft_isspace((unsigned char)line[len - 1]))
		len--;
// TODO : issue33 例外の種類を増やす？
	if (len == 0)
		return (excepiom_argment("texture path is missing"));
	path = ft_substr(line, 0, len);
	if (path == NULL)
		return (malloc_err());
	return (SUCCESS);
}

/**
 * @brief テクスチャパスの形式を検証します。
 *
 * @param path 検証対象のテクスチャパス。
 *
 * @retval true 空白を含まず `.xpm` で終わる正しいパスの場合。
 * @retval false NULL、空白混在、拡張子不正、末尾 `/` の場合。
 */
static t_excepion	is_valid_texture_path(char *path)
{
	t_excepion state;
	if ( state != SUCCESS)
		return (malloc_err());
// TODO : issue33 例外内容増やす
// - texture path is invalid
// - texture path must end with .xpm
	if (ft_strchr(path, ' ') != NULL || ft_strchr(path, '\t') != NULL)
		return (excepiom_argment("texture path is invalid"));
	if (!parse_extension(path, ".xpm") || path[ft_strlen(path) - 1] == '/')
		return (excepiom_argment("texture path must end with .xpm"));
	return (true);
}
