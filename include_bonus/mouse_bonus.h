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

/**
 * @brief マウス視点制御を開始する
 *
 * カーソルを非表示にし,基準位置へ寄せ,内部状態を初期化する.
 * 以後は`mouse_update()`またはLinux上の`handle_mouse_move()`が
 * 基準位置からの差分を視点変化へ変換する.
 *
 * @param[in,out] game マウス状態とMLXコンテキストを持つゲーム状態
 */
void	mouse_capture(t_game *game);

/**
 * @brief マウス視点制御を終了する
 *
 * カーソル表示を元へ戻し,未処理の相対移動量やフラグを破棄する.
 * ウィンドウ破棄や通常終了の経路から安全に重ねて呼べるよう,
 * 未捕捉時は何もせず戻る.
 *
 * @param[in,out] game マウス状態を解放するゲーム状態
 */
void	mouse_release(t_game *game);

/**
 * @brief 1フレーム分のマウス差分をプレイヤー視点へ適用する
 *
 * Linuxでは直前のMotionNotifyで蓄積した差分を消費し,
 * macOSでは現在位置を取得して基準位置との差分をその場で計算する.
 * 視点が実際に動いたかどうかは`game->mouse.moved_this_frame`へ残され,
 * 描画省略判定の材料として使われる.
 *
 * @param[in,out] game プレイヤー視点とマウス状態を更新するゲーム状態
 */
void	mouse_update(t_game *game);

/**
 * @brief Linuxのマウス移動イベントから相対移動量を蓄積する
 *
 * X11のMotionNotifyは「今どこにいるか」を通知するため,
 * このハンドラではウィンドウ中央からのずれを`pending_dx/dy`へ加算する.
 * 実際の視点更新は即時には行わず,次の`mouse_update()`でまとめて適用する.
 *
 * @param[in]     x    イベント時のウィンドウ内x座標
 * @param[in]     y    イベント時のウィンドウ内y座標
 * @param[in,out] game 差分を蓄積するゲーム状態
 * @return 0 (MLXフック規約)
 */
int		handle_mouse_move(int x, int y, t_game *game);

#endif
