#ifndef CUB_CONFIG_H
#define CUB_CONFIG_H

#include <stdbool.h>
#include <stdint.h>


// rgbで床色と天井色を表現するための構造体
// - r,g,b: 赤、緑、青.
//  + 0~255の範囲
// - value: 描画用にまとめた24bitの色値
//  + 'value = (r << 16) | (g << 8) | b;'等で8bitずつ詰めて1つの整数値にする
// - is_set: 実際に入力済みかどうかを表すbool
//  + 黒が有効値なので、'F 0,0,0'のような値は通すので、それに従い設定済みか判定する
typedef struct s_rgb
{
	int	r;
	int	g;
	int	b;
	uint32_t	value;
	bool is_set;
}	t_rgb;

// 方角
typedef enum e_dir
{
	EAST = 0,
	WEST,
	NORTH,
	SOUTH
}t_dir;


// 四方向の壁テクスチャを配列 index で一元管理するための列挙体
// - TEX_NO,TEX_SO,TEX_WE,TEX_EA: 北,南,西,東の壁に対応する
// - TEX_COUNT: 配列長
//  + 壁テクスチャ配列は必ず '0 <= index < TEX_COUNT' を満たす
typedef enum e_tex_id
{
	TEX_NO = 0,
	TEX_SO,
	TEX_WE,
	TEX_EA,
	TEX_COUNT
}	t_tex_id;

// 四方向の壁テクスチャのファイルパス名を保持する構造体
// - path: 壁テクスチャのファイルパス
//  + path[TEX_NO] = NO, path[TEX_SO] = SO, path[TEX_WE] = WE, path[TEX_EA] = EA
typedef struct s_tex_path
{
	char		*path[TEX_COUNT];
}	t_tex_path;

// プレイヤーの初期位置,向きを保持する構造体
// - row: map上の行番号
// - col: map上の列番号
// - dir: プレイヤーの初期向き
//  + 'EAST','WEST','NORTH','SOUTH'の列挙値で設定する
typedef struct s_spawn
{
	int		row;
	int		col;
	t_dir	dir;
}	t_spawn;


// map本体を保持する構造体
// - grid: mapの各行をもつ二次元配列
// - width: mapの最大幅
//  + mapの各行数が同じ長さであるとは限らないので、最長幅を保持する
// - height: mapの行数 
typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
}	t_map;


// '.cub'のパース処理を行い、処理結果を出力する構造体
// - tex: 四方向のテクスチャパス
// - floor_color: 床色
// - ceiling_color: 天井色
// - map: map本体
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
