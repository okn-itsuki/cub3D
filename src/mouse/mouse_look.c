/**
 * @file mouse_look.c
 * @brief FPS風のマウス視点制御
 *
 * @details
 * カーソルをウィンドウ中央へ固定し,毎フレームの相対移動量だけを
 * プレイヤーの左右回転と上下視点オフセットへ反映する.
 * MiniLibXのプラットフォーム差異はこのモジュール内へ閉じ込め,
 * 呼び出し側は capture / release / update の3操作だけを使えばよい.
 */
#include "mouse.h"
#include "game_config.h"
#include "ray_casting.h"

#define MOUSE_CENTER_X (WIN_W / 2)
#define MOUSE_CENTER_Y (WIN_H / 2)
#define MOUSE_YAW_SENSITIVITY 0.002
#define MOUSE_PITCH_SENSITIVITY 1.0
#define MOUSE_MAX_DX (WIN_W / 2)
#define MOUSE_MAX_DY (WIN_H / 2)

/**
 * @brief 縦視点オフセットを画面内へ収まる範囲に制限する
 *
 * 現在の上下視点は,本物の3D pitch回転ではなく
 * 「壁と背景の見かけ上の中心線を上下へずらす」簡易モデルである.
 * そのため許容範囲を画面高さの半分弱へ制限しないと,
 * 地平線が完全に画面外へ出て見た目が破綻しやすい.
 *
 * @param[in] offset_y 候補となる縦視点オフセット [pixel]
 * @return クランプ済みの縦視点オフセット [pixel]
 */
static double	clamp_view_offset_y(double offset_y)
{
	double	max_offset_y;

	max_offset_y = (double)(WIN_H / 2 - 1);
	if (offset_y > max_offset_y)
		return (max_offset_y);
	if (offset_y < -max_offset_y)
		return (-max_offset_y);
	return (offset_y);
}

/**
 * @brief 相対移動量の外れ値を安全な範囲へ丸める
 *
 * フォーカス復帰直後や座標取得異常時に巨大な差分が入ると,
 * 1フレームで大回転や大きな上下移動が起こる.
 * ここでは画面半分を上限にして,異常値がそのまま視点へ乗らないようにする.
 *
 * @param[in] delta 補正前の相対移動量 [pixel]
 * @param[in] limit 絶対値の上限
 * @return クランプ済みの相対移動量
 */
static int	clamp_delta(int delta, int limit)
{
	if (delta > limit)
		return (limit);
	if (delta < -limit)
		return (-limit);
	return (delta);
}

/**
 * @brief カーソルを非表示にする
 *
 * LinuxとmacOSでMiniLibXのAPIシグネチャが異なるため,
 * 呼び出し側がOS分岐を意識しないようこの関数へ閉じ込める.
 *
 * @param[in] game MLX本体とウィンドウを持つゲーム状態
 */
static void	hide_cursor(const t_game *game)
{
#if defined(PLATFORM_LINUX)
	mlx_mouse_hide(game->mlx.mlx, game->mlx.win);
#elif defined(PLATFORM_MACOS)
	(void)game;
	mlx_mouse_hide();
#else
	(void)game;
#endif
}

/**
 * @brief 非表示にしていたカーソルを再表示する
 *
 * `mouse_capture()`で隠したカーソルを通常状態へ戻す.
 * 終了経路から複数回呼ばれても破綻しないよう,
 * 上位では捕捉状態を見てからこの関数を呼ぶ.
 *
 * @param[in] game MLX本体とウィンドウを持つゲーム状態
 */
static void	show_cursor(const t_game *game)
{
#if defined(PLATFORM_LINUX)
	mlx_mouse_show(game->mlx.mlx, game->mlx.win);
#elif defined(PLATFORM_MACOS)
	(void)game;
	mlx_mouse_show();
#else
	(void)game;
#endif
}

/**
 * @brief カーソルを相対移動の基準位置へワープさせる
 *
 * FPS風の操作では「絶対位置」ではなく「前回から何pixel動いたか」を使いたい.
 * そのため各サンプル取得後にカーソルをウィンドウ中央へ戻し,
 * 次回イベントや次回取得で生じるずれを相対移動量として読む.
 *
 * @param[in] game MLX本体とウィンドウを持つゲーム状態
 */
static void	center_cursor(const t_game *game)
{
#if defined(PLATFORM_LINUX)
	mlx_mouse_move(game->mlx.mlx, game->mlx.win,
		MOUSE_CENTER_X, MOUSE_CENTER_Y);
#elif defined(PLATFORM_MACOS)
	mlx_mouse_move(game->mlx.win, MOUSE_CENTER_X, MOUSE_CENTER_Y);
#else
	(void)game;
#endif
}

/**
 * @brief 現在のカーソル座標を取得する
 *
 * Linux版MiniLibXは`mlx_mouse_get_pos()`の戻り値で取得成功を返すため,
 * ここでも`bool`で成功可否を返す.
 * 失敗時は呼び出し側がそのフレームの差分適用を諦めることで,
 * 不定な座標から巨大deltaが生じることを避ける.
 *
 * @param[in]  game MLX本体とウィンドウを持つゲーム状態
 * @param[out] x    取得したx座標の書き込み先
 * @param[out] y    取得したy座標の書き込み先
 * @retval true  取得成功
 * @retval false 取得失敗
 */
static bool	get_cursor_pos(const t_game *game, int *x, int *y)
{
#if defined(PLATFORM_LINUX)
	return (mlx_mouse_get_pos(game->mlx.mlx, game->mlx.win, x, y) != 0);
#elif defined(PLATFORM_MACOS)
	mlx_mouse_get_pos(game->mlx.win, x, y);
	return (true);
#else
	(void)game;
	*x = MOUSE_CENTER_X;
	*y = MOUSE_CENTER_Y;
	return (true);
#endif
}

/**
 * @brief 画面上の相対移動量をプレイヤー視点へ変換する
 *
 * - `dx` は水平回転角 `dx * MOUSE_YAW_SENSITIVITY` [rad] へ変換
 * - `dy` は縦視点オフセット `dy * MOUSE_PITCH_SENSITIVITY` [pixel] へ変換
 *
 * 左右回転は`rotate_player()`へ委譲し,方向ベクトル`dir`とカメラ平面`plane`
 * を同じ角度だけ回す.
 * 上下視点は描画中心線を上下へずらす簡易モデルなので,
 * `player.view_offset_y`へ直接加算してから安全範囲へクランプする.
 *
 * @param[in,out] game 視点更新対象のゲーム状態
 * @param[in]     dx   水平相対移動量 [pixel]
 * @param[in]     dy   垂直相対移動量 [pixel]
 * @retval true  視点が実際に変化した
 * @retval false 変化しなかった
 */
static bool	apply_mouse_delta(t_game *game, int dx, int dy)
{
	bool	changed;

	changed = false;
	dx = clamp_delta(dx, MOUSE_MAX_DX);
	dy = clamp_delta(dy, MOUSE_MAX_DY);
	if (dx != 0)
	{
		rotate_player(&game->player, dx * MOUSE_YAW_SENSITIVITY);
		changed = true;
	}
	if (dy != 0)
	{
		game->player.view_offset_y = clamp_view_offset_y(
				game->player.view_offset_y
				- dy * MOUSE_PITCH_SENSITIVITY);
		changed = true;
	}
	return (changed);
}

void	mouse_capture(t_game *game)
{
	if (game == NULL || game->mlx.mlx == NULL || game->mlx.win == NULL)
		return ;
	if (game->mouse.is_captured)
		return ;
	hide_cursor(game);
	center_cursor(game);
	game->mouse.is_captured = true;
#if defined(PLATFORM_LINUX)
	game->mouse.skip_next_update = false;
#else
	game->mouse.skip_next_update = true;
#endif
	game->mouse.pending_dx = 0;
	game->mouse.pending_dy = 0;
	game->mouse.moved_this_frame = false;
}

void	mouse_release(t_game *game)
{
	if (game == NULL || !game->mouse.is_captured)
		return ;
	show_cursor(game);
	game->mouse.is_captured = false;
	game->mouse.skip_next_update = false;
	game->mouse.pending_dx = 0;
	game->mouse.pending_dy = 0;
	game->mouse.moved_this_frame = false;
}

int	handle_mouse_move(int x, int y, t_game *game)
{
#if defined(PLATFORM_LINUX)
	if (game == NULL || !game->mouse.is_captured)
		return (0);
	if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
		return (0);
	if (x == MOUSE_CENTER_X && y == MOUSE_CENTER_Y)
		return (0);
	game->mouse.pending_dx += x - MOUSE_CENTER_X;
	game->mouse.pending_dy += y - MOUSE_CENTER_Y;
	center_cursor(game);
#else
	(void)x;
	(void)y;
	(void)game;
#endif
	return (0);
}

void	mouse_update(t_game *game)
{
	int	x;
	int	y;

	if (game == NULL || !game->mouse.is_captured
		|| game->mlx.mlx == NULL || game->mlx.win == NULL)
		return ;
	game->mouse.moved_this_frame = false;
#if defined(PLATFORM_LINUX)
	if (game->mouse.pending_dx == 0 && game->mouse.pending_dy == 0)
		return ;
	game->mouse.moved_this_frame = apply_mouse_delta(game,
			game->mouse.pending_dx, game->mouse.pending_dy);
	game->mouse.pending_dx = 0;
	game->mouse.pending_dy = 0;
#else
	x = MOUSE_CENTER_X;
	y = MOUSE_CENTER_Y;
	if (!get_cursor_pos(game, &x, &y))
		return ;
	if (game->mouse.skip_next_update)
	{
		game->mouse.skip_next_update = false;
		if (x != MOUSE_CENTER_X || y != MOUSE_CENTER_Y)
			center_cursor(game);
		return ;
	}
	if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
		return ;
	if (x == MOUSE_CENTER_X && y == MOUSE_CENTER_Y)
		return ;
	game->mouse.moved_this_frame = apply_mouse_delta(game,
			x - MOUSE_CENTER_X, y - MOUSE_CENTER_Y);
	center_cursor(game);
#endif
}
