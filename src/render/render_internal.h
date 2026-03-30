/**
 * @file render_internal.h
 * @brief renderモジュール内部で共有する補助関数宣言
 */
#ifndef RENDER_INTERNAL_H
# define RENDER_INTERNAL_H

# include "ray_casting.h"

void	init_dda_step(t_ray *ray, const t_player *player);

#endif
