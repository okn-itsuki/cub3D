/**
 * @file cub3d.h
 * @brief cub3Dの中心的な型定義 (画像・ベクトル・プレイヤー・レイキャスト・ゲーム状態)
 *
 * @details
 * 本ヘッダは,複数モジュールで共有される中核データ構造を定義する.
 * 2次元ベクトル,画像,プレイヤー,レイキャスト途中状態,ゲーム全体状態を
 * 一元的にまとめることで,関数シグネチャと所有関係を読みやすくしている.
 * 詳しいアーキテクチャ説明とDOT図は`docs/mainpage.dox`側に集約する.
 */
#ifndef CUB3D_H
#define CUB3D_H

#include "cub_config.h"
#include "mlx.h"

#define WIN_W 600	/**< ウィンドウ幅 [pixel] */
#define WIN_H 400	/**< ウィンドウ高さ [pixel] */

/**
 * @struct s_img
 * @brief MLXの画像1枚分のデータを保持する構造体
 *
 * 画像オブジェクト本体と、ピクセル直接書き込みに必要なメタ情報をまとめて持つ。
 */
typedef struct s_img
{
	void	*img;		/**< mlx_new_image / mlx_xpm_file_to_image が返す画像 */
	char	*addr;		/**< mlx_get_data_addr が返すピクセルデータ先頭 */
	int		bpp;		/**< 1ピクセルあたりのビット数 */
	int		line_len;	/**< 1行あたりのバイト数 (パディング含む) */
	int		endian;		/**< エンディアン (0=little, 1=big) */
	int		width;		/**< 画像の横幅 [pixel] */
	int		height;		/**< 画像の縦幅 [pixel] */
}	t_img;

/**
 * @struct s_mlx
 * @brief MLX本体と描画先ウィンドウを保持する構造体
 */
typedef struct s_mlx
{
	void	*mlx;	/**< mlx_init() が返すMLXインスタンス */
	void	*win;	/**< mlx_new_window() が返すウィンドウ */
}	t_mlx;

/**
 * @struct s_vec2d
 * @brief 実数の2次元ベクトル (座標・方向ベクトル用)
 */
typedef struct s_vec2d
{
	double		x;	/**< x成分 */
	double		y;	/**< y成分 */
}	t_vec2d;

/**
 * @struct s_ivec2
 * @brief 整数の2次元ベクトル (マップセル・ステップ符号用)
 */
typedef struct s_ivec2
{
	int			x;	/**< x成分 */
	int			y;	/**< y成分 */
}	t_ivec2;

/**
 * @struct s_input
 * @brief フレーム間で保持するキー入力状態
 */
typedef struct s_input
{
	bool		move_forward;	/**< 前進キー押下中 */
	bool		move_backward;	/**< 後退キー押下中 */
	bool		strafe_left;	/**< 左平行移動キー押下中 */
	bool		strafe_right;	/**< 右平行移動キー押下中 */
	bool		turn_left;		/**< 左回転キー押下中 */
	bool		turn_right;		/**< 右回転キー押下中 */
	bool		quit;			/**< 終了フラグ (一度trueになったら戻らない) */
}	t_input;

/**
 * @struct s_mouse
 * @brief マウス視点制御の実行時状態
 *
 * カーソルを中央固定して相対移動だけを見るFPS風操作のため,
 * 捕捉中かどうかと,中央ワープ直後の1サンプルを無視するフラグを持つ.
 */
typedef struct s_mouse
{
	bool		is_captured;		/**< カーソルを捕捉している */
	bool		skip_next_update;	/**< 次回サンプルを無視する */
	int			pending_dx;			/**< 未適用の水平移動量 [pixel] */
	int			pending_dy;			/**< 未適用の垂直移動量 [pixel] */
	bool		moved_this_frame;	/**< このフレームで視点変化があった */
}	t_mouse;

/**
 * @struct s_player
 * @brief プレイヤーの実行時状態 (毎フレーム更新)
 *
 * - pos: ワールド座標系の現在位置。初期値は `spawn.col+0.5, spawn.row+0.5`
 * - dir: 正規化済み前方ベクトル (`||dir|| = 1`)
 * - plane: カメラ平面ベクトル。dirと直交し `||plane|| = tan(FOV/2)`
 * - view_offset_y: 画面中心からの縦視点オフセット [pixel]
 */
typedef struct s_player
{
	t_vec2d		pos;		/**< ワールド座標での現在位置 */
	t_vec2d		dir;		/**< 正規化済み前方ベクトル */
	t_vec2d		plane;		/**< カメラ平面ベクトル (dirと直交) */
	double		move_speed;	/**< 移動速度 [セル/秒] */
	double		rot_speed;	/**< 回転速度 [rad/秒] */
	double		view_offset_y;	/**< 縦視点オフセット [pixel] */
}	t_player;

/**
 * @struct s_texture
 * @brief 壁テクスチャ1枚分の実画像を保持する構造体
 *
 * ロード済みかどうかの所有状態は`t_assets.wall_mask`で管理する。
 */
typedef struct s_texture
{
	t_img		image;	/**< MLX上にロード済みの画像データ */
}	t_texture;

/**
 * @struct s_assets
 * @brief 描画用アセット (MLXに読み込んだ画像資源)
 *
 * パスは`t_config.tex`が、実画像は本構造体が管理する。
 * `wall_mask`には、部分初期化時にどの壁テクスチャを所有しているかを記録する。
 */
typedef struct s_assets
{
	t_texture	wall[TEX_COUNT];	/**< 四方向の壁テクスチャ */
	uint32_t	wall_mask;			/**< 所有中の壁テクスチャを表すビットマスク */
}	t_assets;

/**
 * @brief DDAでどちらの軸の壁に衝突したかを表す
 */
typedef enum e_hit_side
{
	HIT_X = 0,	/**< x方向の境界で衝突 (東西の壁) */
	HIT_Y		/**< y方向の境界で衝突 (南北の壁) */
}	t_hit_side;

/**
 * @struct s_ray
 * @brief 1列分のレイキャスト計算に使う一時状態
 *
 * 画面の各列(0 <= column < WIN_W)について、DDAアルゴリズムで
 * 壁との衝突を検出するための中間値をすべて保持する。
 *
 * - camera_x: カメラ平面上の正規化座標 (-1.0 ～ +1.0)
 * - ray_dir: `dir + plane * camera_x` で求まるレイ方向
 * - delta_dist: 各軸方向に1セル進むのに必要な距離
 *   - `delta_dist.x = |1 / ray_dir.x|`
 * - step: DDAの各軸の進行方向 (-1 or +1)
 * - wall_x: 壁面上の衝突位置の小数部 (0.0 <= wall_x < 1.0)
 */
typedef struct s_ray
{
	int			column;			/**< 描画中の画面列 (0 <= column < WIN_W) */
	double		camera_x;		/**< カメラ平面上の正規化座標 */
	t_vec2d		ray_dir;		/**< この列のレイ方向ベクトル */
	t_ivec2		map;			/**< DDAが現在参照しているマップセル */
	t_vec2d		side_dist;		/**< 次のグリッド境界までの距離 */
	t_vec2d		delta_dist;		/**< 1セル進むのに必要な距離 */
	t_ivec2		step;			/**< DDAの各軸の進行符号 (-1 or +1) */
	t_hit_side	hit_side;		/**< どちらの軸の壁に衝突したか */
	double		perp_wall_dist;	/**< 魚眼補正済みの壁までの垂直距離 */
	double		wall_x;			/**< 壁面上の衝突位置の小数部 */
}	t_ray;

/**
 * @struct s_column
 * @brief 壁1列分の描画パラメータ
 *
 * - draw_start, draw_end: 0 <= draw_start <= draw_end < WIN_H にクランプ済み
 * - tex_x: 0 <= tex_x < texture.width
 */
typedef struct s_column
{
	int			column;		/**< この列の画面x座標 */
	int			draw_start;	/**< 壁描画開始y座標 */
	int			draw_end;	/**< 壁描画終了y座標 */
	t_tex_id	tex_id;		/**< 使用する壁テクスチャID */
	int			tex_x;		/**< テクスチャ上のx座標 */
	double		tex_step;	/**< 画面y1ピクセルあたりのテクスチャy増分 */
	double		tex_pos;	/**< 現在のテクスチャy位置 (実数) */
}	t_column;

/**
 * @struct s_render
 * @brief 描画フレームとレイキャスト用の作業領域
 */
typedef struct s_render
{
	t_img		frame;	/**< 画面全体を書き込むオフスクリーン画像 */
	t_ray		ray;	/**< 1列分のDDA計算用 */
	t_column	column;	/**< 1列分の描画パラメータ */
	int			view_center_y;	/**< このフレームの見かけの画面中心y */
}	t_render;

/**
 * @struct s_frame_clock
 * @brief フレーム時間の管理構造体
 *
 * 前フレームからの経過時間を保持し、移動量・更新量の計算に使う。
 * `delta_sec = (now_us - last_tick_us) / 1000000.0`
 */
typedef struct s_frame_clock
{
	uint64_t	frame_index;	/**< 開始からのフレーム通番 */
	uint64_t	last_tick_us;	/**< 直前フレームの時刻 [usec] */
	double		delta_sec;		/**< 直前フレームからの経過時間 [sec] */
}	t_frame_clock;

/**
 * @struct s_game
 * @brief cub3D全体の実行状態をまとめるルート構造体
 *
 * @par 初期化順
 * config -> mlx -> win -> frame -> assets.wall[] -> player/input/mouse/clock -> running
 *
 * @par 破棄順
 * assets.wall[] -> render.frame -> mlx.win -> mlx.mlx -> config
 */
typedef struct s_game
{
	t_config		config;			/**< .cubファイルのパース結果 */
	t_mlx			mlx;			/**< MLX本体とウィンドウ */
	t_assets		assets;			/**< 壁テクスチャ画像 */
	t_render		render;			/**< フレーム画像とレイキャスト作業領域 */
	t_player		player;			/**< プレイヤーの実行時状態 */
	t_input			input;			/**< 入力状態 */
	t_mouse			mouse;			/**< マウス視点制御状態 */
	t_frame_clock	clock;			/**< フレーム時間 */
	uint32_t		runtime_mask;	/**< 初期化済み実行時資源を表すビットマスク */
	bool			running;		/**< ゲームループ継続フラグ */
}	t_game;
#endif
