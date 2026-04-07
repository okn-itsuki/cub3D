/**
 * @file render_internal_bonus.h
 * @brief renderモジュール内部で共有する補助関数宣言
 */
#ifndef BONUS_RENDER_INTERNAL_H
# define BONUS_RENDER_INTERNAL_H

# include "ray_casting_bonus.h"

void	init_dda_step(t_ray *ray, const t_player *player);

#endif
