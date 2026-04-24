/**
 * @file parse.c
 * @brief `.cub` ファイル全体を `t_config` へ変換するエントリ
 */
#include "parse.h"

static t_excepion	fill_config_from_lines(char **lines, t_config *config);

/**
 * @brief `.cub` ファイルを読み込み `t_config` を構築します。
 *
 * @param path 読み込む `.cub` ファイルのパス。
 * @param config 成功時にパース済み設定を受け取る出力先。
 *
 * @retval true パースが成功した場合。
 * @retval false 拡張子不正、読み込み失敗、構文エラー、またはメモリ確保失敗時。
 */
t_excepion	parse_cub(const char *path, t_config *config)
{
	char			**lines;
	t_excepion	state;

	if (path == NULL || config == NULL)
		return (excepion_message("parse arguments are invalid",ARG_NULL));
	init_config(config);
	if (!parse_extension(path, ".cub"))
		return (excepion_message("map file must end with .cub",EXTENSION_ERR));
	lines = NULL;
	state = read_file_lines(path, &lines);
	if (state != SUCCESS)
		return (state);
	state = fill_config_from_lines(lines, config);
	if (state != SUCCESS)
	{
		free_lines(lines);
		destroy_config(config);
		return (state);
	}
	free_lines(lines);
	return (SUCCESS);
}

/**
 * @brief 行配列からヘッダ部とマップ部を切り分けて `config` に代入
 *
 * @param lines `read_file_lines()` が返した NULL 終端の行配列。
 * @param config パース結果の格納先。
 *
 * @retval true ヘッダ・マップが正当で `config` のメンバに値を格納しきった場合。
 * @retval false `.cub` のレイアウト不正や要素不足、マップ不正があった場合。
 */
static t_excepion	fill_config_from_lines(char **lines, t_config *config)
{
	int					index;
	int					map_start;
	t_excepion			state;
	t_parse_line_type	type;

	index = 0;
	map_start = -1;
	while (lines[index] != NULL)
	{
		type = parse_get_line_type(lines[index]);
		if (type == LINE_EMPTY)
			index++;
		else if (map_start >= 0)
			break ;
		else if (type == LINE_TEXTURE || type == LINE_COLOR)
		{
			/* ヘッダ 1 行を識別子ごとに解釈する処理。
			 * 必要機能: 要素種別の判定、書式検証、重複検出、
			 * 設定値の保存をまとめて行うこと。
			 */
			state = parse_header_line(lines[index], config);
			if (state != SUCCESS)
				return (state);
			index++;
		}
		/* 必須ヘッダが出そろったかを判定する処理。
		 * 必要機能: マップ開始判定の前提として、
		 * 必須 6 要素の充足状態を一貫して返すこと。
		 */
		else if (parse_all_headers_set(config) == SUCCESS && type == LINE_MAP)
			map_start = index;
		else
			return (excepion_message("invalid .cub layout",Draft));
	}
	if (map_start < 0)
		map_start = index;
	while (lines[index] != NULL && !parse_is_blank_line(lines[index]))
		index++;
	/* 必須ヘッダの最終充足を検証する処理。
	 * 必要機能: マップ解析に進む前に、
	 * 未設定要素の有無を確実に検出すること。
	 */
	if (parse_all_headers_set(config) != SUCCESS)
		return (excepion_message("required identifiers are missing", Draft));
	/* マップ領域を構築し妥当性検証まで完了する処理。
	 * 必要機能: 行の保持、スポーン抽出、文字検証、
	 * 閉塞判定を行い、成功時だけ設定へ反映すること。
	 */
	state = parse_map_lines(lines, map_start, index, config);
	if (state != SUCCESS)
		return (state);
	while (lines[index] != NULL)
	{
		if (!parse_is_blank_line(lines[index]))
			return (excepion_message("content after map is invalid",Draft));
		index++;
	}
	return (SUCCESS);
}
