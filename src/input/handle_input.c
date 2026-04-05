/**
 * @file handle_input.c
 * @brief キー入力の押下/解放ハンドラ・イベントフック登録
 */
#include "game_init.h"
#include "game_config.h"
#include "mouse.h"

/**
 * @brief 指定キーに対応する入力フラグを更新する
 *
 * ESCキーは押下時のみquitフラグを立てる一方通行の扱いとする。
 *
 * @param[in,out] input   更新対象の入力状態
 * @param[in]     keycode 対象キーコード
 * @param[in]     pressed 押下時true,解放時false
 */
static void	set_key_state(t_input *input, int keycode, bool pressed)
{
	if (keycode == KEY_W)
		input->move_forward = pressed;
	else if (keycode == KEY_S)
		input->move_backward = pressed;
	else if (keycode == KEY_A)
		input->strafe_left = pressed;
	else if (keycode == KEY_D)
		input->strafe_right = pressed;
	else if (keycode == KEY_LEFT)
		input->turn_left = pressed;
	else if (keycode == KEY_RIGHT)
		input->turn_right = pressed;
	else if (keycode == KEY_ESC && pressed)
		input->quit = true;
}

/**
 * @brief キー押下イベントのハンドラ
 *
 * @param[in]     keycode 押下されたキーコード
 * @param[in,out] game    入力フラグを更新するゲーム状態
 * @return 0 (MLXフック規約)
 */
int	handle_key_press(int keycode, t_game *game)
{
	if (game != NULL)
		set_key_state(&game->input, keycode, true);
	return (0);
}

/**
 * @brief キー解放イベントのハンドラ
 *
 * @param[in]     keycode 解放されたキーコード
 * @param[in,out] game    入力フラグを更新するゲーム状態
 * @return 0 (MLXフック規約)
 */
int	handle_key_release(int keycode, t_game *game)
{
	if (game != NULL)
		set_key_state(&game->input, keycode, false);
	return (0);
}

/**
 * @brief ウィンドウの閉じるボタン押下イベントのハンドラ
 *
 * @param[in,out] game runningフラグとquitフラグを更新するゲーム状態
 * @return 0 (MLXフック規約)
 */
int	handle_close(t_game *game)
{
	if (game != NULL)
	{
		mouse_release(game);
		game->running = false;
		game->input.quit = true;
	}
	return (0);
}

/**
 * @brief MLXのイベントフックを一括登録する
 *
 * @param[in,out] game フック登録先のゲーム状態
 */
void	register_hooks(t_game *game)
{
	if (game == NULL || game->mlx.win == NULL)
		return ;
	mlx_hook(game->mlx.win, EVENT_KEY_PRESS, MASK_KEY_PRESS,
		handle_key_press, game);
	mlx_hook(game->mlx.win, EVENT_KEY_RELEASE, MASK_KEY_RELEASE,
		handle_key_release, game);
	mlx_hook(game->mlx.win, EVENT_DESTROY, 0, handle_close, game);
	mouse_capture(game);
}
