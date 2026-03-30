/**
 * @file render_column.c
 * @brief 壁1列分の描画処理 (描画範囲計算・テクスチャ選択・ピクセル書き込み)
 */
#include "ray_casting.h"
#include "game_config.h"

/**
 * @brief 壁の垂直距離から描画範囲(draw_start,draw_end)を計算する
 *
 * @param[out] col            描画範囲の書き込み先
 * @param[in]  perp_wall_dist 魚眼補正済みの壁までの垂直距離
 * @return 壁1列の高さ(line_height)。テクスチャ座標計算に使用する。
 */
static int	calc_draw_range(t_column *col, double perp_wall_dist)
{
	int	line_height;

	line_height = (int)(WIN_H / perp_wall_dist);
	if (line_height < 1)
		line_height = 1;
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
 */
static void	choose_texture(t_column *col, const t_ray *ray)
{
	if (ray->hit_side == HIT_X && ray->ray_dir.x < 0)
		col->tex_id = TEX_WE;
	else if (ray->hit_side == HIT_X)
		col->tex_id = TEX_EA;
	else if (ray->ray_dir.y < 0)
		col->tex_id = TEX_NO;
	else
		col->tex_id = TEX_SO;
}

/**
 * @brief テクスチャのx座標・ステップ量・初期y位置を計算する
 */
static void	calc_tex_coords(t_column *col, const t_ray *ray,
		const t_img *tex, int line_height)
{
	col->tex_x = (int)(ray->wall_x * tex->width);
	if (col->tex_x >= tex->width)
		col->tex_x = tex->width - 1;
	if (ray->hit_side == HIT_X && ray->ray_dir.x < 0)
		col->tex_x = tex->width - col->tex_x - 1;
	if (ray->hit_side == HIT_Y && ray->ray_dir.y > 0)
		col->tex_x = tex->width - col->tex_x - 1;
	col->tex_step = (double)tex->height / (double)line_height;
	col->tex_pos = (col->draw_start - WIN_H / 2 + line_height / 2)
		* col->tex_step;
}

/**
 * @brief フレームバッファ上の指定ピクセルへのポインタを返す
 */
static char	*pixel_ptr(t_img *img, int x, int y)
{
	return (img->addr + y * img->line_len
		+ x * (img->bpp / BITS_PER_BYTE));
}

/**
 * @brief 壁部分だけをフレームバッファに描画する
 *
 * 背景は`render_frame.c`側で先に塗りつぶされている前提とする。
 */
static void	draw_wall_range(t_img *frame, t_column *col, const t_img *tex)
{
	int		y;
	int		tex_y;
	char	*dst;

	y = col->draw_start;
	while (y <= col->draw_end)
	{
		tex_y = (int)col->tex_pos;
		if (tex_y >= tex->height)
			tex_y = tex->height - 1;
		dst = pixel_ptr(frame, col->column, y++);
		*(uint32_t *)dst = *(uint32_t *)(tex->addr + tex_y * tex->line_len
				+ col->tex_x * (tex->bpp / BITS_PER_BYTE));
		col->tex_pos += col->tex_step;
	}
}

/**
 * @brief レイキャスト結果から壁1列分をフレームに描画する
 *
 * @param[in,out] render 描画作業領域 (ray結果を参照し,frame/columnを更新)
 * @param[in]     assets 壁テクスチャアセット
 */
void	render_column(t_render *render, const t_assets *assets)
{
	int			line_height;
	t_column	*col;
	const t_img	*tex;

	col = &render->column;
	col->column = render->ray.column;
	line_height = calc_draw_range(col, render->ray.perp_wall_dist);
	choose_texture(col, &render->ray);
	tex = &assets->wall[col->tex_id].image;
	calc_tex_coords(col, &render->ray, tex, line_height);
	draw_wall_range(&render->frame, col, tex);
}
