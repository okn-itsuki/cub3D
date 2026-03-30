/**
 * @file game_loop_tick.c
 * @brief ゲームのメインループ (デルタタイム算出・更新・描画)
 */
#include "ray_casting.h"
#include "game_init.h"
#include <sys/time.h>
#include <stdlib.h>

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
 * @brief mlx_loop_hookから毎フレーム呼ばれるメインループ関数
 *
 * デルタタイムを算出し、プレイヤー更新と1フレーム描画を実行する。
 * 終了条件(quit/!running)または時刻取得失敗時は資源を解放してexitする。
 * デルタタイムは最大0.1秒にクランプし、極端なフレーム落ち時の暴走を防ぐ。
 *
 * @param[in,out] game ゲーム状態
 * @retval true 正常終了
 */
bool	game_loop_tick(t_game *game)
{
	uint64_t	now_us;

	if (game->input.quit || !game->running)
	{
		destroy_game_resources(game);
		destroy_config(&game->config);
		exit(0);
	}
	if (get_current_time_us(&now_us) == false)
	{
		destroy_game_resources(game);
		destroy_config(&game->config);
		exit(0);
	}
	if (game->clock.last_tick_us == 0)
		game->clock.delta_sec = 0.0;
	else
	{
		game->clock.delta_sec = (now_us - game->clock.last_tick_us) * SEC_PER_US;
		if (game->clock.delta_sec > 0.1)
			game->clock.delta_sec = 0.1;
	}
	game->clock.last_tick_us = now_us;
	game->clock.frame_index++;
	update_player(&game->player, &game->input,
		&game->config.map, game->clock.delta_sec);
	render_frame(game);
	return (true);
}
