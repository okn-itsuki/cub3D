/**
 * @file render_frame_bonus.c
 * @brief 1フレーム分の描画 (背景塗りつぶし + 全列レイキャスト + ウィンドウ転送)
 *
 * @details
 * 描画は,背景一括塗りつぶしと列ごとの壁描画に二段階分割されている.
 * これにより,`render_column()`側は壁だけに集中でき,天井/床との責務が混ざらない.
 */
#include "ray_casting_bonus.h"
#include "mlx.h"

/**
 * @brief フレームバッファの1行を単色で塗りつぶす
 */
static void	fill_row(t_img *frame, int y, uint32_t color)
{
	uint32_t	*dst;
	int			x;

	dst = (uint32_t *)(frame->addr + y * frame->line_len);
	x = 0;
	while (x < frame->width)
	{
		dst[x] = color;
		++x;
	}
}

/**
 * @brief フレームバッファ全体を天井色と床色で塗りつぶす
 *
 * 上半分を天井色,下半分を床色にする最も単純な背景モデルを採用している.
 */
static void	fill_background(t_img *frame, uint32_t ceil_color,
		uint32_t floor_color)
{
	int	y;

	y = 0;
	while (y < frame->height / 2)
	{
		fill_row(frame, y, ceil_color);
		++y;
	}
	while (y < frame->height)
	{
		fill_row(frame, y, floor_color);
			++y;
	}
}

/**
 * @brief 全画面列に対してレイキャストと壁描画を実行する
 *
 * 列単位で`cast_ray()`と`render_column()`を直列に呼び,
 * DDA結果をそのまま壁描画へ流す.
 */
static void	render_columns(t_game *game)
{
	int	col;

	col = 0;
	while (col < WIN_W)
	{
		cast_ray(&game->render.ray, &game->player, &game->config.map, col);
		render_column(&game->render, &game->assets);
		++col;
	}
}

/**
 * @brief 1フレームを描画する
 *
 * 背景を塗りつぶした後,全画面列の壁を描画し,フレームバッファをウィンドウに転送する。
 * 更新対象は`game->render`配下へ閉じており,他モジュールへ副作用を広げない.
 *
 * @param[in,out] game ゲーム状態 (render.frame/ray/columnを更新)
 */
void	render_frame(t_game *game)
{
	fill_background(&game->render.frame, game->config.ceiling_color.value,
		game->config.floor_color.value);
	render_columns(game);
	mlx_put_image_to_window(game->mlx.mlx, game->mlx.win,
		game->render.frame.img, 0, 0);
}
