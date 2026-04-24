/**
 * @file parse.c
 * @brief `.cub` ファイル全体を `t_config` へ変換するエントリ
 */
#include "parse.h"

static bool	fill_config_from_lines(char **lines, t_config *config);

/**
 * @brief `.cub` ファイルを読み込み `t_config` を構築します。
 *
 * @param path 読み込む `.cub` ファイルのパス。
 * @param config 成功時にパース済み設定を受け取る出力先。
 *
 * @retval true パースが成功した場合。
 * @retval false 拡張子不正、読み込み失敗、構文エラー、またはメモリ確保失敗時。
 */
bool	parse_cub(const char *path, t_config *config)
{
	char			**lines;
	t_system_err	state;

	if (path == NULL || config == NULL)
		return (parse_put_error("parse arguments are invalid"));
	init_config(config);
	if (!parse_extension(path, ".cub"))
		return (parse_put_error("map file must end with .cub"));
	lines = NULL;
	state = read_file_lines(path, &lines);
	if (state != SUCCESS)
		return (false);
	if (!fill_config_from_lines(lines, config))
	{
		free_lines(lines);
		destroy_config(config);
		return (false);
	}
	free_lines(lines);
	return (true);
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
static bool	fill_config_from_lines(char **lines, t_config *config)
{
	int					index;
	int					map_start;
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
			if (TODO)
				return (false);
			index++;
		}
		/* 必須ヘッダが出そろったかを判定する処理。
		 * 必要機能: マップ開始判定の前提として、
		 * 必須 6 要素の充足状態を一貫して返すこと。
		 */
		else if (TODO && type == LINE_MAP)
			map_start = index;
		else
			return (parse_put_error("invalid .cub layout"));
	}
	if (map_start < 0)
		map_start = index;
	while (lines[index] != NULL && !parse_is_blank_line(lines[index]))
		index++;
	/* 必須ヘッダの最終充足を検証する処理。
	 * 必要機能: マップ解析に進む前に、
	 * 未設定要素の有無を確実に検出すること。
	 */
	if (TODO)
		return (parse_put_error("required identifiers are missing"));
	/* マップ領域を構築し妥当性検証まで完了する処理。
	 * 必要機能: 行の保持、スポーン抽出、文字検証、
	 * 閉塞判定を行い、成功時だけ設定へ反映すること。
	 */
	if (TODO)
		return (false);
	while (lines[index] != NULL)
	{
		if (!parse_is_blank_line(lines[index]))
			return (parse_put_error("content after map is invalid"));
		index++;
	}
	return (true);
}
