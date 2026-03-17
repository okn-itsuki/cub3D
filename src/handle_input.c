#include "cub3d.h"
#include "game_config.h"
#include "game_init.h"

// 何する関数か:
// - `t_input` の全フィールドを `false` で初期化する。
// 参照でいじった値:
// - `input` の全押下フラグを `false` に設定する。
// 戻り値の意味:
// - なし。
void	init_input(t_input *input)
{
	input->move_forward = false;
	input->move_backward = false;
	input->strafe_left = false;
	input->strafe_right = false;
	input->turn_left = false;
	input->turn_right = false;
	input->quit = false;
}

// 何する関数か:
// - キー押下イベントを受け取り、対応する入力フラグを `true` にする。
// 参照でいじった値:
// - `game->input` の各フラグを `true` に設定する。
//   + `KEY_ESC` は `quit` を立てる一方通行フラグ。解放イベントでは落とさない。
// 戻り値の意味:
// - 0 (mlx のフック規約に従う)。
int	handle_key_press(int keycode, t_game *game)
{
	if (keycode == KEY_W)
		game->input.move_forward = true;
	else if (keycode == KEY_S)
		game->input.move_backward = true;
	else if (keycode == KEY_A)
		game->input.strafe_left = true;
	else if (keycode == KEY_D)
		game->input.strafe_right = true;
	else if (keycode == KEY_LEFT)
		game->input.turn_left = true;
	else if (keycode == KEY_RIGHT)
		game->input.turn_right = true;
	else if (keycode == KEY_ESC)
		game->input.quit = true;
	return (0);
}

// 何する関数か:
// - キー解放イベントを受け取り、対応する入力フラグを `false` にする。
// 参照でいじった値:
// - `game->input` の各移動・回転フラグを `false` に設定する。
//   + `quit` は一方通行なので解放時に落とさない。
// 戻り値の意味:
// - 0 (mlx のフック規約に従う)。
int	handle_key_release(int keycode, t_game *game)
{
	if (keycode == KEY_W)
		game->input.move_forward = false;
	else if (keycode == KEY_S)
		game->input.move_backward = false;
	else if (keycode == KEY_A)
		game->input.strafe_left = false;
	else if (keycode == KEY_D)
		game->input.strafe_right = false;
	else if (keycode == KEY_LEFT)
		game->input.turn_left = false;
	else if (keycode == KEY_RIGHT)
		game->input.turn_right = false;
	return (0);
}

// 何する関数か:
// - ウィンドウの閉じるボタン押下イベントを受け取り、ゲームループを終了させる。
// 参照でいじった値:
// - `game->running` を `false` に設定する。
// 戻り値の意味:
// - 0 (mlx のフック規約に従う)。
int	handle_close(t_game *game)
{
	game->running = false;
	return (0);
}

// 何する関数か:
// - mlx のイベントフックを一括登録する。
// 参照でいじった値:
// - なし (mlx 内部にコールバックを登録するだけ)。
// 戻り値の意味:
// - なし。
void	register_hooks(t_game *game)
{
	mlx_hook(game->mlx.win, EVENT_KEY_PRESS, MASK_KEY_PRESS,
		handle_key_press, game);
	mlx_hook(game->mlx.win, EVENT_KEY_RELEASE, MASK_KEY_RELEASE,
		handle_key_release, game);
	mlx_hook(game->mlx.win, EVENT_DESTROY, 0, handle_close, game);
}
