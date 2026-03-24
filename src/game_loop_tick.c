#include "ray_casting.h"
#include "game_init.h"
#include <sys/time.h>
#include <stdlib.h>

#define US_PER_SEC 1000000
#define SEC_PER_US 1e-6

static uint64_t	get_current_time_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((uint64_t)tv.tv_sec * US_PER_SEC + (uint64_t)tv.tv_usec);
}

void	update_player(t_player *player, const t_input *input,
			const t_map *map, double dt);

int	game_loop_tick(t_game *game)
{
	uint64_t	now_us;

	if (game->input.quit || !game->running)
	{
		destroy_game_resources(game);
		exit(0);
	}
	now_us = get_current_time_us();
	game->clock.delta_sec = (now_us - game->clock.last_tick_us) * SEC_PER_US;
	game->clock.last_tick_us = now_us;
	game->clock.frame_index++;
	update_player(&game->player, &game->input,
		&game->config.map, game->clock.delta_sec);
	render_frame(game);
	return (0);
}
