/**
 * @file cub_config_bonus.h
 * @brief .cubファイルのパース結果を保持する型定義
 *
 * @details
 * `t_config`は起動前に確定する静的な設定を表す.
 * 実行中に頻繁に変化する状態は保持せず,テクスチャパス,色,マップ,スポーン位置だけを持つ.
 * 実行時のマップ参照は,行長が不揃いでも安全に扱えるよう
 * `map_cell_at()`/`map_is_solid()`経由に統一している.
 */
#ifndef BONUS_CUB_CONFIG_H
#define BONUS_CUB_CONFIG_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @struct s_rgb
 * @brief 床色・天井色をRGBで表現する構造体
 *
 * 色の各成分(0-255)と、描画用にパックした24bit色値を保持する。
 * `F 0,0,0` のような黒も有効値のため、設定済みフラグを別途管理する。
 */
typedef struct s_rgb
{
	int			r;		/**< 赤成分 (0-255) */
	int			g;		/**< 緑成分 (0-255) */
	int			b;		/**< 青成分 (0-255) */
	uint32_t	value;	/**< パック済み色値 `(r << 16) | (g << 8) | b` */
	bool		is_set;	/**< 値が設定済みかどうか */
}	t_rgb;

/**
 * @brief プレイヤーの初期向きや方位を表す列挙体
 */
typedef enum e_dir
{
	DIR_UNSET = -1,	/**< 未設定 */
	EAST = 0,		/**< 東 */
	WEST,			/**< 西 */
	NORTH,			/**< 北 */
	SOUTH			/**< 南 */
}t_dir;

/**
 * @brief 壁テクスチャIDを表す列挙体
 *
 * 四方向の壁テクスチャを配列indexで一元管理する。
 * 壁テクスチャ配列は `0 <= index < TEX_COUNT` で使用する。
 */
typedef enum e_tex_id
{
	TEX_NO = 0,	/**< 北壁テクスチャ */
	TEX_SO,		/**< 南壁テクスチャ */
	TEX_WE,		/**< 西壁テクスチャ */
	TEX_EA,		/**< 東壁テクスチャ */
	TEX_COUNT	/**< テクスチャの総数 (配列長として使用) */
}	t_tex_id;

/**
 * @struct s_tex_path
 * @brief 四方向の壁テクスチャファイルパスを保持する構造体
 *
 * 設定ファイルから読んだテクスチャパスを方向ごとに保持する。
 * `path[TEX_NO]`=北, `path[TEX_SO]`=南, `path[TEX_WE]`=西, `path[TEX_EA]`=東。
 */
typedef struct s_tex_path
{
	char		*path[TEX_COUNT];	/**< 方向ごとのテクスチャファイルパス */
}	t_tex_path;

/**
 * @struct s_spawn
 * @brief プレイヤーの初期位置と向きを保持する構造体
 */
typedef struct s_spawn
{
	int		row;	/**< マップ上の行番号 */
	int		col;	/**< マップ上の列番号 */
	t_dir	dir;	/**< 初期向き (EAST/WEST/NORTH/SOUTH) */
}	t_spawn;

/**
 * @struct s_map
 * @brief マップ本体を保持する構造体
 *
 * マップの2次元文字列とそのサイズ情報を保持する。
 * 各行の長さは不均一の場合があるため、widthは最長行の幅を保持する。
 * 実行時の参照は`map_cell_at()`/`map_is_solid()`経由を想定する。
 */
typedef struct s_map
{
	char	**grid;		/**< マップの各行を持つ二次元配列 */
	int		width;		/**< マップの最大幅 (最長行の文字数) */
	int		height;		/**< マップの行数 */
}	t_map;

/**
 * @struct s_config
 * @brief .cubファイルのパース結果を保持するルート構造体
 *
 * 実行に必要な設定値をパース済みの形でまとめて持つ。
 */
typedef struct s_config
{
	t_tex_path	tex;			/**< 四方向のテクスチャパス */
	t_rgb		floor_color;	/**< 床色 */
	t_rgb		ceiling_color;	/**< 天井色 */
	t_map		map;			/**< マップ本体 */
	t_spawn		spawn;			/**< プレイヤーの初期位置と向き */
}	t_config;

char	map_cell_at(const t_map *map, int x, int y);

bool	map_is_solid(const t_map *map, int x, int y);

#endif
