#ifndef CUB_MLX_H
#define CUB_MLX_H

#define WIN_W 1280
#define WIN_H 960



// mlxのinitに使用する構造体
// - img: mlx_new_image()からアドレスを取得し、描画バッファの確保をする
// - addr: mlx_get_data_addr()から返り値として先頭アドレスを取得
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


#endif
