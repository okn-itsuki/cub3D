/**
 * @file render_frame.c
 * @brief 1フレーム分の描画 (背景塗りつぶし + 全列レイキャスト + ウィンドウ転送)
 *
 * @details
 * 描画は,背景一括塗りつぶしと列ごとの壁描画に二段階分割されている.
 * これにより,`render_column()`側は壁だけに集中でき,天井/床との責務が混ざらない.
 */
#include "ray_casting.h"
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
		uint32_t floor_color, int split_y)
{
	int	y;

	if (split_y < 0)
		split_y = 0;
	if (split_y > frame->height)
		split_y = frame->height;
	y = 0;
	while (y < split_y)
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
 * @brief 縦視点オフセットを反映した画面中心y座標を返す
 *
 * 画面外へ大きく外れすぎると壁や背景の見え方が破綻しやすいため,
 * 表示可能範囲へクランプして返す.
 */
static int	get_view_center_y(const t_player *player)
{
	int	center_y;

	center_y = WIN_H / 2 + (int)player->view_offset_y;
	if (center_y < 0)
		center_y = 0;
	if (center_y >= WIN_H)
		center_y = WIN_H - 1;
	return (center_y);
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
	game->render.view_center_y = get_view_center_y(&game->player);
	fill_background(&game->render.frame, game->config.ceiling_color.value,
		game->config.floor_color.value, game->render.view_center_y);
	render_columns(game);
	mlx_put_image_to_window(game->mlx.mlx, game->mlx.win,
		game->render.frame.img, 0, 0);
}
