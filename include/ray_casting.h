#ifndef RAY_CASTING_H
#define RAY_CASTING_H

#include "cub3d.h"

void	cast_ray(t_ray *ray, const t_player *player, const t_map *map,
			int col);
void	render_column(t_render *render, const t_assets *assets,
			uint32_t ceil_color, uint32_t floor_color);
void	render_frame(t_game *game);
void	update_player(t_player *player, const t_input *input,
			const t_map *map, double dt);
bool	game_loop_tick(t_game *game);

#endif
