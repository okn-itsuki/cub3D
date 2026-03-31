#ifndef RAY_CASTING_H
#define RAY_CASTING_H

#include "cub3d.h"

typedef enum e_game_tick_status
{
	GAME_TICK_CONTINUE = 0,
	GAME_TICK_STOP,
	GAME_TICK_ERROR
}	t_game_tick_status;

void	cast_ray(t_ray *ray, const t_player *player, const t_map *map,
			int col);
void	render_column(t_render *render, const t_assets *assets);
void	render_frame(t_game *game);
void	update_player(t_player *player, const t_input *input,
			const t_map *map, double dt);
t_game_tick_status	game_loop_tick(t_game *game);

#endif
