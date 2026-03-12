#ifndef CUB3D_H
#define CUB3D_H

#include "cub_config.h"
#include "mlx.h"
#include "libft.h"

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




// 整数の2次元ベクトルを表す構造体
// - x,y: map cell, step, index など離散量の各成分
// - ot_ray が値として所有する
typedef struct s_ivec2
{
	int			x;
	int			y;
}	t_ivec2;


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




// DDAでどちらの軸の壁に衝突したかを表す列挙体
// - HIT_X: x方向の境界で衝突
// - HIT_Y: y方向の境界で衝突
typedef enum e_hit_side
{
	HIT_X = 0,
	HIT_Y
}	t_hit_side;


// 1列分のraycastの計算で使う一時状態を保持する構造体
// - column: いま描画している画面列
//  + 0 <= column < WIN_W
// - camera_x: カメラ平面上の正規化座標
//  + camera_x = 2 * column / (WIN_W - 1) - 1
// - ray_dir: その列に飛ばすray方向
//  + ray_dir = dir + plane * camera_x
// - map: DDAが現在参照しているmap cell
// - side_dist: 次のgrid境界までの距離
// - delta_dist: x/y方向へ1cell進むのに必要な距離
//  + delta_dist.x = |1 / ray_dir.x|
//  + delta_dist.y = |1 / ray_dir.y|
//  + ray_dir成分が0のときは0除算を避けて INFINITY 相当で扱う
// - step: DDAで各軸に進む符号
//  + step.x, step.y は -1 または +1
// - hit_side: どちらの軸の壁に当たったか
// - perp_wall_dist: 魚眼補正後の壁までの垂直距離
// - wall_x: 壁面上の衝突位置の小数部
//  + 0.0 <= wall_x < 1.0 を保つ
typedef struct s_ray
{
	int			column;
	double		camera_x;
	t_vec2d		ray_dir;
	t_ivec2		map;
	t_vec2d		side_dist;
	t_vec2d		delta_dist;
	t_ivec2		step;
	t_hit_side	hit_side;
	double		perp_wall_dist;
	double		wall_x;
}	t_ray;


// 壁1列の描画結果を保持する構造体
// - draw_start: 壁の描画開始y座標
// - draw_end: 壁の描画終了y座標
//  + 0 <= draw_start <= draw_end < WIN_H に clamp する
// - tex_id: 使用する壁テクスチャID
// - tex_x: テクスチャのx座標
//  + 0 <= tex_x < texture.width
// - tex_step: 画面yを1進めたときに進むテクスチャy量
// - tex_pos: 現在のテクスチャyの実数位置
//  + perp_wall_dist = (hit_side == HIT_X)
//  +     ? side_dist.x - delta_dist.x
//  +     : side_dist.y - delta_dist.y
//  + line_height = WIN_H / perp_wall_dist
// - t_render が毎列の描画結果として保持するstate
typedef struct s_column
{
	int			draw_start;
	int			draw_end;
	t_tex_id	tex_id;
	int			tex_x;
	double		tex_step;
	double		tex_pos;
}	t_column;


// 描画フレームとraycastをまとめる構造体
// - frame: 画面全体を書き込むオフスクリーン画像
// - ray: 1列分のDDA計算用 
// - column: 1列分の描画結果 
typedef struct s_render
{
	t_img		frame;
	t_ray		ray;
	t_column	column;
}	t_render;




// フレーム時間を管理する構造体
// - frame_index: 開始からのフレーム数
// - last_tick_us: 直前フレームの時刻[usec]
// - delta_sec: 直前フレームからの経過時間[sec]
//  + delta_sec = (now_us - last_tick_us) / 1000000.0
//  + 異常に大きい値は実装側で clamp して移動量の暴走を防ぐ
typedef struct s_frame_clock
{
	uint64_t	frame_index;
	uint64_t	last_tick_us;
	double		delta_sec;
}	t_frame_clock;

// cub3d全体の実行状態を保持するルート構造体
// - config: '.cub' のパース結果
// - mlx: MLX本体とwindow
// - assets: 壁テクスチャ画像のアセット
// - render: frame画像とraycast
// - player: プレイヤーの実行時状態
// - input: 入力状態
// - clock: フレーム時間
// - init_mask: 部分初期化済みの資源を示すbit集合(変更するかも)
// - running: loop継続中かどうか
// - フロー:
//  + config -> mlx -> win -> frame -> assets.wall[] -> player/input/clock -> running
// - 解放する順序:
//  + assets.wall[] -> render.frame -> mlx.win -> mlx.mlx -> config
typedef struct s_game
{
	t_config		config;
	t_mlx			mlx;
	t_assets		assets;
	t_render		render;
	t_player		player;
	t_input			input;
	t_frame_clock	clock;
	uint32_t		init_mask;
	bool			running;
}	t_game;
#endif
