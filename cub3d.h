#ifndef CUB3D_H
#define CUB3D_H

#define WIN_W 1280
#define WIN_H 960

#include "cub_config.h"

// mlxの画像一枚を表す構造体
// - img: mlx_new_image() または mlx_xpm_file_to_image() が返す画像オブジェクト
// - addr: mlx_get_data_addr()が返す画像データの先頭アドレス
// - bpp: mlx_get_data_addr()が1ピクセルあたりのbit数を書き込む
// - line_len: mlx_get_data_addr()が画像に対しての１行分のbyte数を書き込む
// - endian: mlx_get_data_addr()がエンディアン情報を書き込む
//  + endian = 0: little_endian
//  + endian = 1: big_endian
// - width,heidht: この画像の横,縦幅(pixel数)
//  + mlx_new_image()に渡した値,または mlx_xpm_file_to_image()が返した値を保持する 
typedef struct s_img{
	void *img;
	char *addr;
	int bpp;
	int line_len;
	int endian;
	int width;
	int height;
}t_img;

// - mlx: mlx_init()が返すmlxインスタンス
// - win: mlx_new_window()が返すwindow
typedef struct s_mlx{
	void *mlx;
	void *win;
}t_mlx;




//t_game構造体は修正するかもです
// cub3d全体の実行状態を保持する構造体
// - config: '.cub'のパースの結果
// - mlx: mlx本体とwindow
// - frame: フレーム画像
// - tex_*: 壁のテクスチャ
typedef struct s_game{
	t_config config;
	t_mlx mlx;
	t_img frame;
	t_img tex_no;
	t_img tex_so;
	t_img tex_we;
	t_img tex_ea;
}t_game ;
 

#endif
