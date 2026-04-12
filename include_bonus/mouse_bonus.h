/**
 * @file mouse_bonus.h
 * @brief マウス視点制御の公開API
 *
 * @details
 * このヘッダは,マウスを使った視点更新の入口だけを公開する.
 * 実装側はプラットフォームごとの差異や座標取得方法を内部へ閉じ込め,
 * 呼び出し側は
 *
 * - `mouse_capture()`
 * - `mouse_update()`
 * - `mouse_release()`
 *
 * の3段階で扱えるようにする.
 *
 * LinuxではMotionNotifyを入口にしたイベント駆動,macOSでは毎フレームの
 * 相対移動取得という別経路を持つが,その差異はこの公開APIの外へ漏らさない.
 */
#ifndef BONUS_MOUSE_H
# define BONUS_MOUSE_H

# include "cub3d_bonus.h"

void	mouse_capture(t_game *game);

void	mouse_release(t_game *game);

void	mouse_update(t_game *game);


int		handle_mouse_move(int x, int y, t_game *game);

#endif
