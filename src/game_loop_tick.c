#include "ray_casting.h"
#include "game_init.h"
#include <sys/time.h>
#include <stdlib.h>

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

// 何する関数か:
// - `mlx_loop_hook` から毎フレーム呼ばれるメインループ関数。
// - delta time を算出し、プレイヤー更新と描画を1フレーム分実行する。
// 参照でいじった値:
// - `game->clock` の `delta_sec`, `last_tick_us`, `frame_index` を更新する。
// - `game->player` を `update_player` 経由で更新する。
// - `game->render.frame` を `render_frame` 経由で更新する。
// 戻り値の意味:
// - `true`: 正常終了。
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
