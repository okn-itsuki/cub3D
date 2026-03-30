/**
 * @file render_frame.c
 * @brief 1フレーム分の描画 (全列レイキャスト + ウィンドウ転送)
 */
#include "ray_casting.h"
#include "mlx.h"

/**
 * @brief 1フレームを描画する
 *
 * 全画面列(0 ～ WIN_W-1)に対してレイキャストを実行し、
 * 列ごとに天井/壁/床を描画した後、フレームバッファをウィンドウに転送する。
 *
 * @param[in,out] game ゲーム状態 (render.frame/ray/columnを更新)
 */
void	render_frame(t_game *game)
{
	int	col;

	col = 0;
	while (col < WIN_W)
	{
		cast_ray(&game->render.ray, &game->player,
			&game->config.map, col);
		render_column(&game->render, &game->assets,
			game->config.ceiling_color.value,
			game->config.floor_color.value);
		col++;
	}
	mlx_put_image_to_window(game->mlx.mlx, game->mlx.win,
		game->render.frame.img, 0, 0);
}
