#ifndef CUB3D_H
#define CUB3D_H

#include "cub_config.h"
#include "mlx.h"

#define WIN_W 1280
#define WIN_H 960

// MLXの画像1枚を表す構造体
// 役割:
// - 画像オブジェクト本体と、pixel書き込みに必要な情報をまとめて持つ。
// 主な値:
// - img: `mlx_new_image()` または `mlx_xpm_file_to_image()` が返す画像オブジェクト
// - addr: `mlx_get_data_addr()` が返す画像データの先頭アドレス
// - bpp: `mlx_get_data_addr()` が書き込む 1pixel あたりの bit 数
// - line_len: `mlx_get_data_addr()` が書き込む 1行あたりの byte 数
// - endian: `mlx_get_data_addr()` が書き込むエンディアン情報
//   + 0: little endian
//   + 1: big endian
// - width,height: この画像の横幅と縦幅
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

// MLX本体と描画先windowを表す構造体
// 役割:
// - `mlx_init()` と `mlx_new_window()` の返り値をまとめて持つ。
// 主な値:
// - mlx: MLX本体のインスタンス
// - win: 画面表示用のwindow
typedef struct s_mlx
{
	void	*mlx;
	void	*win;
}	t_mlx;



// 実数の2次元ベクトルを表す構造体
// 役割:
// - 座標や方向ベクトルの実数値を表す。
// 主な値:
// - x: x成分
// - y: y成分
typedef struct s_vec2d
{
	double		x;
	double		y;
}	t_vec2d;




// 整数の2次元ベクトルを表す構造体
// 役割:
// - map cell や step などの離散量を表す。
// 主な値:
// - x: x成分
// - y: y成分
typedef struct s_ivec2
{
	int			x;
	int			y;
}	t_ivec2;


// 入力状態をフレーム間で保持する構造体
// 役割:
// - 各操作キーの押下状態をフレーム間で保持する。
// 主な値:
// - move_forward,move_backward: 前進,後退の押下状態
// - strafe_left,strafe_right: 平行移動の押下状態
// - turn_left,turn_right: 回転の押下状態
// - quit: `true` になったら終了する
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
// 役割:
// - プレイヤーの位置、向き、移動速度を実行時に保持する。
// 主な値:
// - pos: ワールド座標系の現在位置
//   + 初期値は `spawn.col + 0.5`, `spawn.row + 0.5`
// - dir: 正規化済みの前方ベクトル
//   + `||dir|| = 1`
// - plane: 画面平面ベクトル
//   + `dir` と直交する
//   + `||plane|| = tan(FOV / 2)`
// - move_speed: 1秒あたりの移動量
// - rot_speed: 1秒あたりの回転角[rad]
typedef struct s_player
{
	t_vec2d		pos;
	t_vec2d		dir;
	t_vec2d		plane;
	double		move_speed;
	double		rot_speed;
}	t_player;




// 壁テクスチャ1枚の実画像を保持する構造体
// 役割:
// - 壁テクスチャ1枚分の実画像とロード状態を保持する。
// 主な値:
// - image: MLX上にロード済みの画像
// - loaded: 正常にロード済みかどうか
//   + 部分初期化失敗時は `loaded == true` のものだけ破棄する
typedef struct s_texture
{
	t_img		image;
	bool		loaded;
}	t_texture;



// 描画用アセットをまとめる構造体
// 役割:
// - 実際に MLX 上へ読み込んだ画像資源を保持する。
// 主な値:
// - wall: 四方向の壁テクスチャ
//  + `0 <= index < TEX_COUNT` の index だけを使う
//  + パスは `t_config.tex`、実画像は `t_assets.wall` が管理元になる
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
// 役割:
// - 画面1列ぶんの raycast 計算で使う一時状態を保持する。
// 主な値:
// - column: いま描画している画面列
//   + `0 <= column < WIN_W`
// - camera_x: カメラ平面上の正規化座標
//   + `camera_x = 2 * column / (WIN_W - 1) - 1`
// - ray_dir: その列に飛ばす ray 方向
//   + `ray_dir = dir + plane * camera_x`
// - map: DDA が現在参照している map cell
// - side_dist: 次の grid 境界までの距離
// - delta_dist: x/y方向へ1cell進むのに必要な距離
//   + `delta_dist.x = |1 / ray_dir.x|`
//   + `delta_dist.y = |1 / ray_dir.y|`
// - step: DDA で各軸に進む符号
//   + `step.x`, `step.y` は `-1` または `+1`
// - hit_side: どちらの軸の壁に当たったか
// - perp_wall_dist: 魚眼補正後の壁までの垂直距離
// - wall_x: 壁面上の衝突位置の小数部
//   + `0.0 <= wall_x < 1.0` を保つ
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
// 役割:
// - 壁1列ぶんの描画結果を保持する。
// 主な値:
// - column: この列の画面x座標
//   + `0 <= column < WIN_W`
// - draw_start: 壁の描画開始y座標
// - draw_end: 壁の描画終了y座標
//   + `0 <= draw_start <= draw_end < WIN_H` に clamp する
// - tex_id: 使用する壁テクスチャID
// - tex_x: テクスチャのx座標
//   + `0 <= tex_x < texture.width`
// - tex_step: 画面yを1進めたときに進むテクスチャy量
// - tex_pos: 現在のテクスチャyの実数位置
typedef struct s_column
{
	int			column;
	int			draw_start;
	int			draw_end;
	t_tex_id	tex_id;
	int			tex_x;
	double		tex_step;
	double		tex_pos;
}	t_column;


// 描画フレームと raycast 用の作業領域をまとめる構造体
// 役割:
// - 1フレーム描画に必要な画像と一時状態を保持する。
// 主な値:
// - frame: 画面全体を書き込む off-screen 画像
// - ray: 1列分の DDA 計算用
// - column: 1列分の描画結果
typedef struct s_render
{
	t_img		frame;
	t_ray		ray;
	t_column	column;
}	t_render;




// フレーム時間を管理する構造体
// 役割:
// - 前フレームからの経過時間を持ち、移動量や更新量の計算に使う。
// 主な値:
// - frame_index: 開始からのフレーム数
// - last_tick_us: 直前フレームの時刻[usec]
// - delta_sec: 直前フレームからの経過時間[sec]
//   + `delta_sec = (now_us - last_tick_us) / 1000000.0`
typedef struct s_frame_clock
{
	uint64_t	frame_index;
	uint64_t	last_tick_us;
	double		delta_sec;
}	t_frame_clock;

// cub3d全体の実行状態をまとめるルート構造体
// 役割:
// - パース結果、MLX資源、描画状態、入力状態をひとまとめに持つ。
// 主な値:
// - config: `.cub` のパース結果
// - mlx: MLX本体とwindow
// - assets: 壁テクスチャ画像のアセット
// - render: frame画像とraycast用の作業領域
// - player: プレイヤーの実行時状態
// - input: 入力状態
// - clock: フレーム時間
// - init_mask: 部分初期化済みの資源を表す bit 集合
// - running: loop を継続するかどうか
// 初期化順:
// - config -> mlx -> win -> frame -> assets.wall[] -> player/input/clock -> running
// runtime資源の解放順:
// - assets.wall[] -> render.frame -> mlx.win -> mlx.mlx
// - config の解放は parser/sandbox 側の責務で別途行う
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
