/**
 * @file game_loop_tick_bonus.c
 * @brief ゲームのメインループ (デルタタイム算出・更新・描画)
 *
 * @details
 * このモジュールは,「1フレームだけゲームを進める」という責務に限定する.
 * 直接終了処理を持たず,戻り値で上位へ状態を返すことで,loop hook側と責務分離する.
 */
#include "ray_casting_bonus.h"
#include "game_init_bonus.h"
#include "mouse_bonus.h"
#include <sys/time.h>

#define US_PER_SEC 1000000	/**< 1秒あたりのマイクロ秒数 */
#define SEC_PER_US 1e-6		/**< 1マイクロ秒あたりの秒数 */

/**
 * @brief 現在時刻をマイクロ秒単位で取得する
 *
 * @param[out] now_us 現在時刻 [usec] の書き込み先
 * @retval true  取得成功
 * @retval false gettimeofdayが失敗
 */
static bool	get_current_time_us(uint64_t *now_us)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (false);
	*now_us = (uint64_t)tv.tv_sec * US_PER_SEC + (uint64_t)tv.tv_usec;
	return (true);
}

/**
 * @brief フレーム時間情報を更新する
 *
 * 初回フレームではdelta_secを0にし、それ以降は前回tickとの差分を秒に変換する。
 * 極端なフレーム落ち時の暴走を防ぐため、delta_secは最大0.1秒にクランプする。
 * これにより,一時停止やブレークポイント復帰直後の巨大dtで
 * プレイヤーが一気にワープすることを防いでいる.
 *
 * @param[in,out] clock  更新対象のフレーム時計
 * @param[in]     now_us 現在時刻 [usec]
 */
static void	update_delta_time(t_frame_clock *clock, uint64_t now_us)
{
	if (clock->last_tick_us == 0)
		clock->delta_sec = 0.0;
	else
	{
		clock->delta_sec = (now_us - clock->last_tick_us) * SEC_PER_US;
		if (clock->delta_sec > 0.1)
			clock->delta_sec = 0.1;
	}
	clock->last_tick_us = now_us;
	++clock->frame_index;
}

/**
 * @brief 現在時刻を取得してフレーム時計を更新する
 *
 * @param[in,out] clock 更新対象のフレーム時計
 * @retval true  時刻取得と更新に成功
 * @retval false gettimeofdayが失敗
 */
static bool	update_frame_clock(t_frame_clock *clock)
{
	uint64_t	now_us;

	if (!get_current_time_us(&now_us))
		return (false);
	update_delta_time(clock, now_us);
	return (true);
}

/**
 * @brief 継続的な描画更新が必要な入力があるかどうかを返す
 *
 * 移動/回転キーが押下中なら,プレイヤー状態が毎フレーム変わりうるため
 * 再描画を省略しない.
 * 逆に,全キーが離されていてマウス差分も無いフレームでは,
 * 壁投影結果・背景・プレイヤー座標のいずれも変わらないため
 * `render_frame()`を呼ばずCPU負荷を下げられる.
 *
 * @param[in] input 現在の入力状態
 * @retval true  継続描画が必要
 * @retval false 入力由来の再描画は不要
 */
static bool	has_active_input(const t_input *input)
{
	return (input->move_forward || input->move_backward
		|| input->strafe_left || input->strafe_right
		|| input->turn_left || input->turn_right);
}

/**
 * @brief 1フレーム分の更新と描画を行う
 *
 * 終了要求があれば停止を返し、時刻取得に失敗した場合は異常終了を返す。
 * 正常時はdelta timeを更新し、プレイヤー更新と描画を1フレーム分実行する。
 * 返り値駆動にすることで,終了コードや片付け方は呼び出し元が選べる.
 *
 * 描画は常に毎フレーム行うのではなく,次のいずれかを満たしたときだけ実行する。
 *
 * - 初回フレームでまだ画面が一度も出ていない
 * - キー入力によりプレイヤー状態が継続的に変化しうる
 * - マウス差分がこのフレームで視点へ適用された
 *
 * これにより,特にLinux/X11環境で「無入力でも毎フレーム再描画する」負荷を避ける.
 *
 * @param[in,out] game ゲーム状態
 * @retval GAME_TICK_CONTINUE 正常に1フレーム処理した
 * @retval GAME_TICK_STOP     終了要求を検知した
 * @retval GAME_TICK_ERROR    時刻取得などで失敗した
 */
t_game_tick_status	game_loop_tick(t_game *game)
{
	bool	should_render;

	if (game->input.quit || !game->running)
		return (GAME_TICK_STOP);
	if (!update_frame_clock(&game->clock))
		return (GAME_TICK_ERROR);
	mouse_update(game);
	update_player(&game->player, &game->input,
		&game->config.map, game->clock.delta_sec);
	should_render = (game->clock.frame_index == 1
			|| has_active_input(&game->input)
			|| game->mouse.moved_this_frame);
	if (should_render)
		render_frame(game);
	return (GAME_TICK_CONTINUE);
}
