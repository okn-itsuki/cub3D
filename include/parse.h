#ifndef PARSE_H
#define PARSE_H

#include <stdbool.h>
#include "cub_config.h"

// 概要 : .cub ファイルをパースして config に格納する
// 参照で変更された値 : config の全フィールド
// 戻り値の意味 : 成功なら true、失敗なら false
// エラー時の処理 : stderr に "Error\n" + 詳細メッセージを出力して false を返す
bool	parse_cub(const char *path, t_config *config);

// 概要 : t_config が保持するヒープ領域をすべて解放する
// 参照で変更された値 : config->tex.path[], config->map.grid
// メモリ開放義務 : parse_cub が成功した後、呼び出し元が責任を持って呼ぶ
void	free_config(t_config *config);

#endif
