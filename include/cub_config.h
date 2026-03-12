#ifndef CUB_CONFIG_H
#define CUB_CONFIG_H

#include <stdbool.h>
#include <stdint.h>


// RGBで床色と天井色を表現する構造体
// 役割:
// - 色の各成分と、描画用にまとめた色値を保持する。
// 主な値:
// - r,g,b: 赤、緑、青
//   + `0` から `255` の範囲
// - value: 描画用にまとめた 24bit の色値
//   + `value = (r << 16) | (g << 8) | b`
// - is_set: 実際に入力済みかどうか
//   + `F 0,0,0` のような黒も有効値なので、別で管理する
typedef struct s_rgb
{
	int	r;
	int	g;
	int	b;
	uint32_t	value;
	bool is_set;
}	t_rgb;

// 方角を表す列挙体
// 役割:
// - プレイヤーの初期向きや方位を表す。
// 主な値:
// - EAST,WEST,NORTH,SOUTH: 東西南北の4方向
typedef enum e_dir
{
	EAST = 0,
	WEST,
	NORTH,
	SOUTH
}t_dir;


// 壁テクスチャIDを表す列挙体
// 役割:
// - 四方向の壁テクスチャを配列 index で一元管理する。
// 主な値:
// - TEX_NO,TEX_SO,TEX_WE,TEX_EA: 北,南,西,東の壁に対応する
// - TEX_COUNT: 配列長
//   + 壁テクスチャ配列は `0 <= index < TEX_COUNT` を満たす
typedef enum e_tex_id
{
	TEX_NO = 0,
	TEX_SO,
	TEX_WE,
	TEX_EA,
	TEX_COUNT
}	t_tex_id;

// 四方向の壁テクスチャのファイルパスを保持する構造体
// 役割:
// - 設定ファイルから読んだテクスチャパスを方向ごとに保持する。
// 主な値:
// - path: 壁テクスチャのファイルパス配列
//   + `path[TEX_NO] = NO`, `path[TEX_SO] = SO`
//   + `path[TEX_WE] = WE`, `path[TEX_EA] = EA`
typedef struct s_tex_path
{
	char		*path[TEX_COUNT];
}	t_tex_path;

// プレイヤーの初期位置と向きを保持する構造体
// 役割:
// - map 上のスポーン位置と初期方角を保持する。
// 主な値:
// - row: map上の行番号
// - col: map上の列番号
// - dir: プレイヤーの初期向き
//   + `EAST`, `WEST`, `NORTH`, `SOUTH` のいずれか
typedef struct s_spawn
{
	int		row;
	int		col;
	t_dir	dir;
}	t_spawn;


// map本体を保持する構造体
// 役割:
// - map の2次元文字列と、そのサイズ情報を保持する。
// 主な値:
// - grid: map の各行をもつ二次元配列
// - width: map の最大幅
//   + 各行の長さが同じとは限らないので、最長幅を保持する
// - height: map の行数
typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
}	t_map;


// `.cub` のパース結果を保持する構造体
// 役割:
// - 実行に必要な設定値を、パース済みの形でまとめて持つ。
// 主な値:
// - tex: 四方向のテクスチャパス
// - floor_color: 床色
// - ceiling_color: 天井色
// - map: map 本体
// - spawn: プレイヤーの初期位置と向き
typedef struct s_config
{
	t_tex_path	tex;
	t_rgb		floor_color;
	t_rgb		ceiling_color;
	t_map		map;
	t_spawn		spawn;
}	t_config;

#endif
