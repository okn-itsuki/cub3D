#include "game_init.h"

void	init_input(t_input *input)
{
	if (input == NULL)
		return ;
	input->move_forward = false;
	input->move_backward = false;
	input->strafe_left = false;
	input->strafe_right = false;
	input->turn_left = false;
	input->turn_right = false;
	input->quit = false;
}
