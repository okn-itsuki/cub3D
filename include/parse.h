#ifndef PARSE_H
#define PARSE_H

#include <stdbool.h>
#include "cub_config.h"

/**
 * @brief .cub ファイルをパースして config に格納する
 * @param path 読み込む .cub ファイルパス
 * @param config パース結果を書き込む設定構造体
 * @return 成功時は true、失敗時は false
 */
bool	parse_cub(const char *path, t_config *config);

/**
 * @brief t_config が保持するヒープ領域をすべて解放する
 * @param config 解放対象の設定構造体
 */
void	free_config(t_config *config);

#endif
