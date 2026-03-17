#include "cub3d.h"
#include "game_config.h"
#include "game_init.h"

void	init_input(t_input *input)
{
	input->move_forward = false;
	input->move_backward = false;
	input->strafe_left = false;
	input->strafe_right = false;
	input->turn_left = false;
	input->turn_right = false;
	input->quit = false;
}

int	handle_key_press(int keycode, t_game *game)
{
	if (keycode == KEY_W)
		game->input.move_forward = true;
	else if (keycode == KEY_S)
		game->input.move_backward = true;
	else if (keycode == KEY_A)
		game->input.strafe_left = true;
	else if (keycode == KEY_D)
		game->input.strafe_right = true;
	else if (keycode == KEY_LEFT)
		game->input.turn_left = true;
	else if (keycode == KEY_RIGHT)
		game->input.turn_right = true;
	else if (keycode == KEY_ESC)
		game->input.quit = true;
	return (0);
}

int	handle_key_release(int keycode, t_game *game)
{
	if (keycode == KEY_W)
		game->input.move_forward = false;
	else if (keycode == KEY_S)
		game->input.move_backward = false;
	else if (keycode == KEY_A)
		game->input.strafe_left = false;
	else if (keycode == KEY_D)
		game->input.strafe_right = false;
	else if (keycode == KEY_LEFT)
		game->input.turn_left = false;
	else if (keycode == KEY_RIGHT)
		game->input.turn_right = false;
	return (0);
}

int	handle_close(t_game *game)
{
	game->running = false;
	return (0);
}

void	register_hooks(t_game *game)
{
	mlx_hook(game->mlx.win, 2, 1L << 0, handle_key_press, game);	// KeyPress
	mlx_hook(game->mlx.win, 3, 1L << 1, handle_key_release, game);	// KeyRelease
	mlx_hook(game->mlx.win, 17, 0, handle_close, game);			// DestroyNotify
}
