#ifndef GAME_MLX_INIT_H
#define GAME_MLX_INIT_H


#include "cub3d.h"

typedef enum e_game_mlx_init_mask
{
	GAME_MLX_READY = 1u << 0,
	GAME_WINDOW_READY = 1u << 1,
	GAME_FRAME_READY = 1u << 2
}	t_game_mlx_init_mask;

bool	init_game_mlx(t_game *game_state);

#endif
