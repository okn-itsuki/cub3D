/**
 * @file mouse_look.c
 * @brief FPS風のマウス視点制御
 *
 * @details
 * カーソルをウィンドウ中央へ固定し,毎フレームの相対移動量だけを
 * プレイヤーの左右回転と上下視点オフセットへ反映する.
 * MiniLibXのプラットフォーム差異はこのモジュール内へ閉じ込め,
 * 呼び出し側は capture / release / update の3操作だけを使えばよい.
 */
#include "mouse.h"
#include "game_config.h"
#include "ray_casting.h"

#define MOUSE_CENTER_X (WIN_W / 2)
#define MOUSE_CENTER_Y (WIN_H / 2)
#define MOUSE_YAW_SENSITIVITY 0.002
#define MOUSE_PITCH_SENSITIVITY 1.0

static double	clamp_view_offset_y(double offset_y)
{
	double	max_offset_y;

	max_offset_y = (double)(WIN_H / 2 - 1);
	if (offset_y > max_offset_y)
		return (max_offset_y);
	if (offset_y < -max_offset_y)
		return (-max_offset_y);
	return (offset_y);
}

static void	hide_cursor(const t_game *game)
{
#if defined(PLATFORM_LINUX)
	mlx_mouse_hide(game->mlx.mlx, game->mlx.win);
#elif defined(PLATFORM_MACOS)
	(void)game;
	mlx_mouse_hide();
#else
	(void)game;
#endif
}

static void	show_cursor(const t_game *game)
{
#if defined(PLATFORM_LINUX)
	mlx_mouse_show(game->mlx.mlx, game->mlx.win);
#elif defined(PLATFORM_MACOS)
	(void)game;
	mlx_mouse_show();
#else
	(void)game;
#endif
}

static void	center_cursor(const t_game *game)
{
#if defined(PLATFORM_LINUX)
	mlx_mouse_move(game->mlx.mlx, game->mlx.win,
		MOUSE_CENTER_X, MOUSE_CENTER_Y);
#elif defined(PLATFORM_MACOS)
	mlx_mouse_move(game->mlx.win, MOUSE_CENTER_X, MOUSE_CENTER_Y);
#else
	(void)game;
#endif
}

static void	get_cursor_pos(const t_game *game, int *x, int *y)
{
#if defined(PLATFORM_LINUX)
	mlx_mouse_get_pos(game->mlx.mlx, game->mlx.win, x, y);
#elif defined(PLATFORM_MACOS)
	mlx_mouse_get_pos(game->mlx.win, x, y);
#else
	(void)game;
	*x = MOUSE_CENTER_X;
	*y = MOUSE_CENTER_Y;
#endif
}

void	mouse_capture(t_game *game)
{
	if (game == NULL || game->mlx.mlx == NULL || game->mlx.win == NULL)
		return ;
	if (game->mouse.is_captured)
		return ;
	hide_cursor(game);
	center_cursor(game);
	game->mouse.is_captured = true;
	game->mouse.skip_next_update = true;
}

void	mouse_release(t_game *game)
{
	if (game == NULL || !game->mouse.is_captured)
		return ;
	show_cursor(game);
	game->mouse.is_captured = false;
	game->mouse.skip_next_update = false;
}

void	mouse_update(t_game *game)
{
	int	x;
	int	y;
	int	dx;
	int	dy;

	if (game == NULL || !game->mouse.is_captured
		|| game->mlx.mlx == NULL || game->mlx.win == NULL)
		return ;
	x = MOUSE_CENTER_X;
	y = MOUSE_CENTER_Y;
	get_cursor_pos(game, &x, &y);
	center_cursor(game);
	if (game->mouse.skip_next_update)
	{
		game->mouse.skip_next_update = false;
		return ;
	}
	if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
		return ;
	dx = x - MOUSE_CENTER_X;
	dy = y - MOUSE_CENTER_Y;
	if (dx != 0)
		rotate_player(&game->player, dx * MOUSE_YAW_SENSITIVITY);
	if (dy != 0)
	{
		game->player.view_offset_y = clamp_view_offset_y(
				game->player.view_offset_y
				- dy * MOUSE_PITCH_SENSITIVITY);
	}
}
