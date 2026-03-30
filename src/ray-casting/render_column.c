/**
 * @file render_column.c
 * @brief 壁1列分の描画処理 (描画範囲計算・テクスチャ選択・ピクセル書き込み)
 */
#include "ray_casting.h"
#include "game_config.h"

/**
 * @brief 壁の垂直距離から描画範囲 (draw_start, draw_end) を計算する
 *
 * 壁の高さを画面中央に配置し、画面外にはみ出す部分をクランプする。
 *
 * @param[out] col            描画範囲の書き込み先
 * @param[in]  perp_wall_dist 魚眼補正済みの壁までの垂直距離
 * @return 壁1列の高さ (line_height)。テクスチャ座標計算に使用する。
 */
static int	calc_draw_range(t_column *col, double perp_wall_dist)
{
	int	line_height;

	line_height = (int)(WIN_H / perp_wall_dist);
	col->draw_start = WIN_H / 2 - line_height / 2;
	if (col->draw_start < 0)
		col->draw_start = 0;
	col->draw_end = WIN_H / 2 + line_height / 2;
	if (col->draw_end >= WIN_H)
		col->draw_end = WIN_H - 1;
	return (line_height);
}

/**
 * @brief 衝突面とレイ方向から使用する壁テクスチャを決定する
 *
 * HIT_X: レイが西向き→西壁、東向き→東壁。
 * HIT_Y: レイが北向き→北壁、南向き→南壁。
 *
 * @param[out] col 選択されたテクスチャIDの書き込み先
 * @param[in]  ray レイキャスト結果 (hit_sideとray_dirを参照)
 */
static void	choose_texture(t_column *col, const t_ray *ray)
{
	if (ray->hit_side == HIT_X)
	{
		if (ray->ray_dir.x < 0)
			col->tex_id = TEX_WE;
		else
			col->tex_id = TEX_EA;
	}
	else
	{
		if (ray->ray_dir.y < 0)
			col->tex_id = TEX_NO;
		else
			col->tex_id = TEX_SO;
	}
}

/**
 * @brief テクスチャのx座標・ステップ量・初期y位置を計算する
 *
 * wall_xからテクスチャU座標を求め、衝突面とレイ方向に応じて
 * 左右反転を行う。ステップ量はテクスチャ高さと壁描画高さの比率。
 *
 * @param[in,out] col    テクスチャ座標の書き込み先
 * @param[in]     ray    レイキャスト結果 (wall_x/hit_side/ray_dirを参照)
 * @param[in]     tex    使用するテクスチャ画像
 * @param[in]     line_h 壁1列の描画高さ
 */
static void	calc_tex_coords(t_column *col, const t_ray *ray,
		const t_img *tex, int line_h)
{
	col->tex_x = (int)(ray->wall_x * tex->width);
	if (col->tex_x >= tex->width)
		col->tex_x = tex->width - 1;
	if (ray->hit_side == HIT_X && ray->ray_dir.x < 0)
		col->tex_x = tex->width - col->tex_x - 1;
	if (ray->hit_side == HIT_Y && ray->ray_dir.y > 0)
		col->tex_x = tex->width - col->tex_x - 1;
	col->tex_step = (double)tex->height / (double)line_h;
	col->tex_pos = (col->draw_start - WIN_H / 2 + line_h / 2)
		* col->tex_step;
}

/**
 * @brief 1列の天井・壁・床をフレームバッファに1パスで書き込む
 *
 * y=0～draw_start: 天井色、draw_start～draw_end: テクスチャ、
 * draw_end～WIN_H: 床色の順でピクセルを書き込む。
 *
 * @param[in,out] frame  書き込み先のフレームバッファ
 * @param[in,out] col    描画パラメータ (tex_posがピクセルごとに進む)
 * @param[in]     tex    壁テクスチャ画像
 * @param[in]     colors colors[0]=天井色, colors[1]=床色
 */
static void	draw_column_stripe(t_img *frame, t_column *col,
		const t_img *tex, uint32_t *colors)
{
	int		y;
	int		tex_y;
	int		tex_x_off;
	char	*dst;

	tex_x_off = col->tex_x * (tex->bpp / BITS_PER_BYTE);
	dst = frame->addr + col->column * (frame->bpp / BITS_PER_BYTE);
	y = 0;
	while (y < col->draw_start)
	{
		*(uint32_t *)dst = colors[0];
		dst += frame->line_len;
		++y;
	}
	while (y <= col->draw_end)
	{
		tex_y = (int)col->tex_pos;
		if (tex_y >= tex->height)
			tex_y = tex->height - 1;
		col->tex_pos += col->tex_step;
		*(uint32_t *)dst
			= *(uint32_t *)(tex->addr + tex_y * tex->line_len + tex_x_off);
		dst += frame->line_len;
		++y;
	}
	while (y < WIN_H)
	{
		*(uint32_t *)dst = colors[1];
		dst += frame->line_len;
		++y;
	}
}

/**
 * @brief レイキャスト結果から1列を天井/壁/床まとめてフレームに描画する
 *
 * @param[in,out] render      描画作業領域 (ray結果を参照し、frame/columnを更新)
 * @param[in]     assets      壁テクスチャアセット
 * @param[in]     ceil_color  天井色 (RGB32)
 * @param[in]     floor_color 床色 (RGB32)
 */
void	render_column(t_render *render, const t_assets *assets,
		uint32_t ceil_color, uint32_t floor_color)
{
	int			line_height;
	t_column	*col;
	const t_img	*tex;
	uint32_t	colors[2];

	col = &render->column;
	col->column = render->ray.column;
	line_height = calc_draw_range(col, render->ray.perp_wall_dist);
	choose_texture(col, &render->ray);
	tex = &assets->wall[col->tex_id].image;
	calc_tex_coords(col, &render->ray, tex, line_height);
	colors[0] = ceil_color;
	colors[1] = floor_color;
	draw_column_stripe(&render->frame, col, tex, colors);
}
