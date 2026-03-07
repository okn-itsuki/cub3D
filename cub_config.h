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


// 四方向の壁のファイルパス名を保持する構造体
typedef struct s_tex_path
{
	char	*no;
	char	*so;
	char	*we;
	char	*ea;
}	t_tex_path;

// プレイヤーの初期位置,向きを保持する構造体
// - row: map上の行番号
// - col: map上の列番号
// - dir: プレイヤーの初期向き
//  + 'EAST','WEST','NORTH','SOUTH'の列挙値で設定する
// - count: map中にspawnがいくつ見つかったか
//  + .cubの開始地点は一つでなければいけないので、
//  + 'count == 0': 開始地点がない
//  + 'count == 1': 正常なケース
//  + 'count > 1 ': 複数開始地点があって不正 
typedef struct s_spawn
{
	int		row;
	int		col;
	t_dir	dir;
	int		count;
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
// - floor: 床色
// - ceiling: 天井色
// - map: map本体
// - spawn: プレイヤーの初期位置と向き
typedef struct s_config
{
	t_tex_path	tex;
	t_rgb		floor;
	t_rgb		ceiling;
	t_map		map;
	t_spawn		spawn;
}	t_config;

#endif
