#include "ray_casting.h"
#include "mlx.h"

// 何する関数か:
// - frame 画像の指定座標に1ピクセルを書き込む。
// 参照でいじった値:
// - `img->addr` の該当オフセットに `color` を書き込む。
// 戻り値の意味:
// - なし。
static void	put_pixel(t_img *img, int x, int y, uint32_t color)
{
	int	offset;

	offset = y * img->line_len + x * (img->bpp / 8);
	*(uint32_t *)(img->addr + offset) = color;
}

// 何する関数か:
// - 画面上半分を天井色で塗りつぶす。
// 参照でいじった値:
// - `frame->addr` の上半分のピクセルを書き換える。
// 戻り値の意味:
// - なし。
static void	draw_ceiling(t_img *frame, uint32_t color)
{
	int	x;
	int	y;

	y = 0;
	while (y < WIN_H / 2)
	{
		x = 0;
		while (x < WIN_W)
		{
			put_pixel(frame, x, y, color);
			x++;
		}
		y++;
	}
}

// 何する関数か:
// - 画面下半分を床色で塗りつぶす。
// 参照でいじった値:
// - `frame->addr` の下半分のピクセルを書き換える。
// 戻り値の意味:
// - なし。
static void	draw_floor(t_img *frame, uint32_t color)
{
	int	x;
	int	y;

	y = WIN_H / 2;
	while (y < WIN_H)
	{
		x = 0;
		while (x < WIN_W)
		{
			put_pixel(frame, x, y, color);
			x++;
		}
		y++;
	}
}

// 何する関数か:
// - 1フレームを描画する。天井・床を塗り、全列をraycastし、windowに転送する。
// 参照でいじった値:
// - `game->render.frame` の全ピクセルを書き換える。
// - `game->render.ray`, `game->render.column` を各列で上書きする。
// 戻り値の意味:
// - なし。
void	render_frame(t_game *game)
{
	int	col;

	draw_ceiling(&game->render.frame, game->config.ceiling_color.value);
	draw_floor(&game->render.frame, game->config.floor_color.value);
	col = 0;
	while (col < WIN_W)
	{
		cast_ray(&game->render.ray, &game->player,
			&game->config.map, col);
		render_column(&game->render, &game->assets);
		col++;
	}
	mlx_put_image_to_window(game->mlx.mlx, game->mlx.win,
		game->render.frame.img, 0, 0);
}
