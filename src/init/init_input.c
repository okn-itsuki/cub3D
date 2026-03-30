/**
 * @file init_input.c
 * @brief 入力状態の初期化
 */
#include "game_init.h"

/**
 * @brief 入力状態の全フィールドをfalseで初期化する
 *
 * @param[out] input 初期化する入力状態
 */
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
