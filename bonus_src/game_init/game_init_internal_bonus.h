/**
 * @file game_init_internal_bonus.h
 * @brief game_initモジュール内部でのみ使う補助関数宣言
 */
#ifndef BONUS_GAME_INIT_INTERNAL_H
# define BONUS_GAME_INIT_INTERNAL_H

# include "game_init_bonus.h"

void	reset_texture_slot(t_texture *texture);
void	destroy_texture_slot(void *mlx_instance, t_texture *texture);
bool	load_texture_slot(t_texture *texture, void *mlx_instance,
				char *texture_path);
void	destroy_mlx_display(void *mlx_instance);

#endif
