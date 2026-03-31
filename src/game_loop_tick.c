#include "ray_casting.h"
#include "game_init.h"
#include <sys/time.h>

#define US_PER_SEC 1000000
#define SEC_PER_US 1e-6

// 何する関数か:
// - 現在時刻をマイクロ秒単位で取得する。
// 参照でいじった値:
// - `*now_us` に現在時刻[usec]を書き込む。
// 戻り値の意味:
// - `true`: 取得成功。
// - `false`: `gettimeofday` が失敗した。
static bool	get_current_time_us(uint64_t *now_us)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (false);
	*now_us = (uint64_t)tv.tv_sec * US_PER_SEC + (uint64_t)tv.tv_usec;
	return (true);
}

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

static bool	update_frame_clock(t_frame_clock *clock)
{
	uint64_t	now_us;

	if (!get_current_time_us(&now_us))
		return (false);
	update_delta_time(clock, now_us);
	return (true);
}

t_game_tick_status	game_loop_tick(t_game *game)
{
	if (game->input.quit || !game->running)
		return (GAME_TICK_STOP);
	if (!update_frame_clock(&game->clock))
		return (GAME_TICK_ERROR);
	update_player(&game->player, &game->input,
		&game->config.map, game->clock.delta_sec);
	render_frame(game);
	return (GAME_TICK_CONTINUE);
}
