/**
 * @file game_loop_tick.c
 * @brief ゲームのメインループ (デルタタイム算出・更新・描画)
 *
 * @details
 * このモジュールは,「1フレームだけゲームを進める」という責務に限定する.
 * 直接終了処理を持たず,戻り値で上位へ状態を返すことで,loop hook側と責務分離する.
 */
#include "ray_casting.h"
#include "game_init.h"
#include "mouse.h"
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
 * @brief 1フレーム分の更新と描画を行う
 *
 * 終了要求があれば停止を返し、時刻取得に失敗した場合は異常終了を返す。
 * 正常時はdelta timeを更新し、プレイヤー更新と描画を1フレーム分実行する。
 * 返り値駆動にすることで,終了コードや片付け方は呼び出し元が選べる.
 *
 * @param[in,out] game ゲーム状態
 * @retval GAME_TICK_CONTINUE 正常に1フレーム処理した
 * @retval GAME_TICK_STOP     終了要求を検知した
 * @retval GAME_TICK_ERROR    時刻取得などで失敗した
 */
t_game_tick_status	game_loop_tick(t_game *game)
{
	if (game->input.quit || !game->running)
		return (GAME_TICK_STOP);
	if (!update_frame_clock(&game->clock))
		return (GAME_TICK_ERROR);
	mouse_update(game);
	update_player(&game->player, &game->input,
		&game->config.map, game->clock.delta_sec);
	render_frame(game);
	return (GAME_TICK_CONTINUE);
}
