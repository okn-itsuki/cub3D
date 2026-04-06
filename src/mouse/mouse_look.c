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
#define MOUSE_MAX_DX (WIN_W / 2)
#define MOUSE_MAX_DY (WIN_H / 2)

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

static int	clamp_delta(int delta, int limit)
{
	if (delta > limit)
		return (limit);
	if (delta < -limit)
		return (-limit);
	return (delta);
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

static bool	get_cursor_pos(const t_game *game, int *x, int *y)
{
#if defined(PLATFORM_LINUX)
	return (mlx_mouse_get_pos(game->mlx.mlx, game->mlx.win, x, y) != 0);
#elif defined(PLATFORM_MACOS)
	mlx_mouse_get_pos(game->mlx.win, x, y);
	return (true);
#else
	(void)game;
	*x = MOUSE_CENTER_X;
	*y = MOUSE_CENTER_Y;
	return (true);
#endif
}

static bool	apply_mouse_delta(t_game *game, int dx, int dy)
{
	bool	changed;

	changed = false;
	dx = clamp_delta(dx, MOUSE_MAX_DX);
	dy = clamp_delta(dy, MOUSE_MAX_DY);
	if (dx != 0)
	{
		rotate_player(&game->player, dx * MOUSE_YAW_SENSITIVITY);
		changed = true;
	}
	if (dy != 0)
	{
		game->player.view_offset_y = clamp_view_offset_y(
				game->player.view_offset_y
				- dy * MOUSE_PITCH_SENSITIVITY);
		changed = true;
	}
	return (changed);
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
#if defined(PLATFORM_LINUX)
	game->mouse.skip_next_update = false;
#else
	game->mouse.skip_next_update = true;
#endif
	game->mouse.pending_dx = 0;
	game->mouse.pending_dy = 0;
	game->mouse.moved_this_frame = false;
}

void	mouse_release(t_game *game)
{
	if (game == NULL || !game->mouse.is_captured)
		return ;
	show_cursor(game);
	game->mouse.is_captured = false;
	game->mouse.skip_next_update = false;
	game->mouse.pending_dx = 0;
	game->mouse.pending_dy = 0;
	game->mouse.moved_this_frame = false;
}

int	handle_mouse_move(int x, int y, t_game *game)
{
#if defined(PLATFORM_LINUX)
	if (game == NULL || !game->mouse.is_captured)
		return (0);
	if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
		return (0);
	if (x == MOUSE_CENTER_X && y == MOUSE_CENTER_Y)
		return (0);
	game->mouse.pending_dx += x - MOUSE_CENTER_X;
	game->mouse.pending_dy += y - MOUSE_CENTER_Y;
	center_cursor(game);
#else
	(void)x;
	(void)y;
	(void)game;
#endif
	return (0);
}

void	mouse_update(t_game *game)
{
	int	x;
	int	y;

	if (game == NULL || !game->mouse.is_captured
		|| game->mlx.mlx == NULL || game->mlx.win == NULL)
		return ;
	game->mouse.moved_this_frame = false;
#if defined(PLATFORM_LINUX)
	if (game->mouse.pending_dx == 0 && game->mouse.pending_dy == 0)
		return ;
	game->mouse.moved_this_frame = apply_mouse_delta(game,
			game->mouse.pending_dx, game->mouse.pending_dy);
	game->mouse.pending_dx = 0;
	game->mouse.pending_dy = 0;
#else
	x = MOUSE_CENTER_X;
	y = MOUSE_CENTER_Y;
	if (!get_cursor_pos(game, &x, &y))
		return ;
	if (game->mouse.skip_next_update)
	{
		game->mouse.skip_next_update = false;
		if (x != MOUSE_CENTER_X || y != MOUSE_CENTER_Y)
			center_cursor(game);
		return ;
	}
	if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
		return ;
	if (x == MOUSE_CENTER_X && y == MOUSE_CENTER_Y)
		return ;
	game->mouse.moved_this_frame = apply_mouse_delta(game,
			x - MOUSE_CENTER_X, y - MOUSE_CENTER_Y);
	center_cursor(game);
#endif
}
