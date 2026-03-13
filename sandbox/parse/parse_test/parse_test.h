#ifndef PARSE_TEST_H
#define PARSE_TEST_H

#include "cub_config.h"

/*
** 役割: パース結果を標準出力へ表示するデバッグ関数群
** - t_config の各フィールドを読みやすい形式で出力する。
** - パーサー実装後に print_config() を呼ぶことで内容を目視確認できる。
*/

void	print_tex_paths(const t_tex_path *tex);
void	print_colors(const t_rgb *floor_color, const t_rgb *ceiling_color);
void	print_map(const t_map *map);
void	print_spawn(const t_spawn *spawn);
void	print_config(const t_config *config);

#endif
