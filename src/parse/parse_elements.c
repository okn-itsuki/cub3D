/**
 * @file parse_elements.c
 * @brief ヘッダ行の振り分けと必須要素の充足確認
 */
#include "parse.h"

/**
 * @brief ヘッダ行 1 行を識別して適切な解析関数へ振り分けます。
 *
 * @param line 解析対象のヘッダ行。
 * @param config 解析結果の格納先。
 *
 * @retval INVALID_STRING マップエラー重複、欠落、不正パス
 * @retval MLLOC_FAIL　メモリ確保失敗時。
 * @retval SUCCESS textureの格納に成功
 */
t_excepion	parse_header_line(const char *line, t_config *config)
{
	t_parse_line_type	type;
	t_excepion			state;

	type = parse_get_line_type(line);
	if (type == LINE_TEXTURE)
		return (parse_texture_line(line, config));
// TODO : color parse 実装
	if (type == LINE_COLOR)
		return (parse_color_line(line, config));
// TODO : 例外内容増やす 
	return (excepion_argment("invalid header line"));
}

/**
 * @brief 必須ヘッダ 6 要素がすべて設定済みか判定します。
 *
 * @param config 判定対象の設定構造体。
 * @retval INVALID_STRING テクスチャ 4 枚と床・天井色いずれかが未設定の場合。
 * @retval MLLOC_FAIL　メモリ確保失敗時。
 * @retval SUCCESS 格納に成功
 */
t_excepion parse_all_headers_set(const t_config *config)
{
	int	index;

	index = 0;
	while (index < TEX_COUNT)
	{
// TODO : issue33 
		if (config->tex.path[index] == NULL)
			return (excepiom_texture("Required texture images are missing.\n"));
		index++;
	}
	if (!config->floor_color.is_set )
		return (excepiom_texture("Floor color has not been specified."));
	if (!config->ceiling_color.is_set)
		return (excepiom_texture("Ceiling color has not been specified."));
	return (SUCCESS);
}
