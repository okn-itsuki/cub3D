/**
 * @file game_config_bonus.h
 * @brief ゲーム全体の定数定義 (レイキャスト・プレイヤー・イベント・キーコード)
 */
#ifndef BONUS_GAME_CONFIG_H
#define BONUS_GAME_CONFIG_H

/** @name レイキャスト定数
 * @{
 */
#define BITS_PER_BYTE 8				/**< 1バイトあたりのビット数 */
#define MIN_PERP_WALL_DIST   1e-4	/**< 壁距離の最小値 (ゼロ除算防止) */
#define CAMERA_X_MIN         (-1.0)	/**< カメラ平面の左端座標 */
#define CAMERA_X_SCALE       2.0	/**< カメラ平面の幅 (左端-1.0 ～ 右端+1.0) */
#define RAY_DELTA_INF        1e30	/**< レイ方向成分が0のときの代替距離 */
#define MAP_WALL_CELL        '1'	/**< マップ上で壁を表す文字 */
/** @} */

/** @name プレイヤー定数
 * @{
 */
#define PLAYER_MOVE_SPEED 3.0	/**< 移動速度 [セル/秒] */
#define PLAYER_ROT_SPEED 3.0	/**< 回転速度 [rad/秒] */
#define FOV_HALF_TAN 0.6		/**< 視野角の半分のtan値 (FOV約66度) */
#define COLLISION_MARGIN 0.2	/**< 壁との衝突判定マージン [セル] */
/** @} */

/** @name X11イベント定数
 * @{
 */
#define EVENT_KEY_PRESS      2		/**< キー押下イベント */
#define EVENT_KEY_RELEASE    3		/**< キー解放イベント */
#define EVENT_DESTROY        17		/**< ウィンドウ破棄イベント */
#define MASK_KEY_PRESS       (1L << 0)	/**< キー押下イベントマスク */
#define MASK_KEY_RELEASE     (1L << 1)	/**< キー解放イベントマスク */
/** @} */

/** @name プラットフォーム判定
 * @{
 */
# if defined(__linux__)
#  define PLATFORM_LINUX 1
# elif defined(__APPLE__) && defined(__MACH__)
#  define PLATFORM_MACOS 1
# endif
/** @} */

/** @name プラットフォーム別キーコード定義
 * @{
 */
# if defined(PLATFORM_LINUX)

#  include <X11/keysym.h>

#  define KEY_ESC       XK_Escape
#  define KEY_LEFT      XK_Left
#  define KEY_RIGHT     XK_Right
#  define KEY_UP        XK_Up
#  define KEY_DOWN      XK_Down
#  define KEY_W         XK_w
#  define KEY_A         XK_a
#  define KEY_S         XK_s
#  define KEY_D         XK_d

# elif defined(PLATFORM_MACOS)

#  define KEY_ESC       53
#  define KEY_LEFT      123
#  define KEY_RIGHT     124
#  define KEY_DOWN      125
#  define KEY_UP        126
#  define KEY_A         0
#  define KEY_S         1
#  define KEY_D         2
#  define KEY_W         13

# endif
/** @} */

#endif
