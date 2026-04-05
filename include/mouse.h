/**
 * @file mouse.h
 * @brief マウス視点制御の公開API
 */
#ifndef MOUSE_H
# define MOUSE_H

# include "cub3d.h"

void	mouse_capture(t_game *game);
void	mouse_release(t_game *game);
void	mouse_update(t_game *game);

#endif
