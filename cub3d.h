#ifndef CUB3D_H
#define CUB3D_H

#include "cub_config.h"

#define WIN_W 1280
#define WIN_H 960

// mlxの画像一枚を表す構造体
// - img: mlx_new_image() または mlx_xpm_file_to_image() が返す画像オブジェクト
// - addr: mlx_get_data_addr() が返す画像データの先頭アドレス
// - bpp: mlx_get_data_addr() が1ピクセルあたりのbit数を書き込む
// - line_len: mlx_get_data_addr() が画像に対しての1行分のbyte数を書き込む
// - endian: mlx_get_data_addr() がエンディアン情報を書き込む
//  + endian = 0: little endian
//  + endian = 1: big endian
// - width,height: この画像の横幅,縦幅(pixel数)
//  + mlx_new_image() に渡した値、または mlx_xpm_file_to_image() が返した値を保持する
typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_img;

// mlx本体とwindowを表す構造体
// - mlx: mlx_init() が返すmlxインスタンス
// - win: mlx_new_window() が返すwindow
typedef struct s_mlx
{
	void	*mlx;
	void	*win;
}	t_mlx;



// 実数の2次元ベクトルを表す構造体
// - x,y: 二次元ベクトルの各座標
// - プレイヤーの位置, 向き, 視野平面を表すために使う
typedef struct s_vec2d
{
	double		x;
	double		y;
}	t_vec2d;




// 入力状態をフレーム間で保持する構造体
// - move_forward,move_backward: 前進,後退の押下状態
// - strafe_left,strafe_right: 平行移動の押下状態
// - turn_left,turn_right: 回転の押下状態
// - quit: trueになったら終了
typedef struct s_input
{
	bool		move_forward;
	bool		move_backward;
	bool		strafe_left;
	bool		strafe_right;
	bool		turn_left;
	bool		turn_right;
	bool		quit;
}	t_input;


// プレイヤーの実行時状態をフレームごとに更新する構造体
// - pos: ワールド座標系の現在位置
//  + 今どこにいるか
//  + 初期値は 'spawn.col + 0.5', 'spawn.row + 0.5'
// - dir: 正規化済みの前方ベクトル
//  + どっちを向いているか
//  + ||dir|| = 1
// - plane: 画面平面ベクトル
//  + 画面としてどの範囲を見ているか
//  + dir ・ plane = 0
//  + ||plane|| = tan(FOV / 2)
//  + 既定 FOV = 66° とすると ||plane|| = tan(33°) ≈ 0.6494075932
// - move_speed: 1秒あたりの移動量
// - rot_speed: 1秒あたりの回転角(rad)
typedef struct s_player
{
	t_vec2d		pos;
	t_vec2d		dir;
	t_vec2d		plane;
	double		move_speed;
	double		rot_speed;
}	t_player;




// 壁テクスチャ1枚の実画像を保持する構造体
// - image: MLX上にロード済みの画像
// - loaded: 正常にロード済みかどうか
//  + 部分初期化失敗時は 'loaded == true' のものだけ破棄する
typedef struct s_texture
{
	t_img		image;
	bool		loaded;
}	t_texture;



// 描画のアセットをまとめる構造体
// - wall: 四方向の壁テクスチャ
//  + '0 <= index < TEX_COUNT' を満たす index だけを使う
//  + path の source of truth は t_config.tex、実画像の source of truth は t_assets.wall
typedef struct s_assets
{
	t_texture	wall[TEX_COUNT];
}	t_assets;


// cub3d全体の実行状態を保持する構造体
// - config: '.cub'のパースの結果
// - mlx: mlx本体とwindow
// - player: プレイヤーの実行時状態
// - frame: フレーム画像
// - tex_*: 四方向の壁テクスチャ画像
//  + 現段階では各方向を個別に保持したいと思います.
typedef struct s_game
{
	t_config	config;
	t_mlx		mlx;
	t_player	player;
	t_img		frame;
	t_img		tex_no;
	t_img		tex_so;
	t_img		tex_we;
	t_img		tex_ea;
}	t_game;

#endif
