#include "ray_casting.h"
#include "game_config.h"
// 何する関数か:
// - 壁の垂直距離から描画範囲 (draw_start, draw_end) を計算する。
// 参照でいじった値:
// - `col->draw_start`, `col->draw_end` を設定する。
// 戻り値の意味:
// - 壁1列の高さ (line_height) を返す。テクスチャ座標計算に使う。
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

// 何する関数か:
// - hit_side と ray 方向からどの壁テクスチャを使うか決める。
// 参照でいじった値:
// - `col->tex_id` を設定する。
// 戻り値の意味:
// - なし。
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

// 何する関数か:
// - テクスチャの x 座標、ステップ量、初期 y 位置を計算する。
// 参照でいじった値:
// - `col->tex_x`, `col->tex_step`, `col->tex_pos` を設定する。
// 戻り値の意味:
// - なし。
static void	calc_tex_coords(t_column *col, const t_ray *ray,
		const t_img *tex, int line_h)
{
	col->tex_x = (int)(ray->wall_x * tex->width);
	if (col->tex_x >= tex->width)
		col->tex_x = tex->width - 1;
	if (ray->hit_side == HIT_X && ray->ray_dir.x > 0)
		col->tex_x = tex->width - col->tex_x - 1;
	if (ray->hit_side == HIT_Y && ray->ray_dir.y < 0)
		col->tex_x = tex->width - col->tex_x - 1;
	col->tex_step = (double)tex->height / (double)line_h;
	col->tex_pos = (col->draw_start - WIN_H / 2 + line_h / 2)
		* col->tex_step;
}

// 何する関数か:
// - テクスチャからピクセルを読み、frame バッファの1列に書き込む。
// 参照でいじった値:
// - `frame->addr` の該当ピクセルを書き換える。
// - `col->tex_pos` をピクセルごとに進める。
// 戻り値の意味:
// - なし。
static void	draw_wall_stripe(t_img *frame, t_column *col,
		const t_img *tex, int x)
{
	int			y;
	int			tex_y;
	int			src_off;
	int			dst_off;
	uint32_t	color;

	y = col->draw_start;
	while (y <= col->draw_end)
	{
		tex_y = (int)col->tex_pos;
		if (tex_y >= tex->height)
			tex_y = tex->height - 1;
		col->tex_pos += col->tex_step;
		src_off = tex_y * tex->line_len + col->tex_x * (tex->bpp / BITS_PER_BYTE);
		color = *(uint32_t *)(tex->addr + src_off);
		dst_off = y * frame->line_len + x * (frame->bpp / BITS_PER_BYTE);
		*(uint32_t *)(frame->addr + dst_off) = color;
		y++;
	}
}

// 何する関数か:
// - ray の結果から壁1列をテクスチャ付きで frame に描画する。
// 参照でいじった値:
// - `render->column` の全フィールドを設定する。
// - `render->frame` の該当ピクセルを書き換える。
// 戻り値の意味:
// - なし。
void	render_column(t_render *render, const t_assets *assets)
{
	int			line_height;
	t_column	*col;
	const t_img	*tex;

	col = &render->column;
	line_height = calc_draw_range(col, render->ray.perp_wall_dist);
	choose_texture(col, &render->ray);
	tex = &assets->wall[col->tex_id].image;
	calc_tex_coords(col, &render->ray, tex, line_height);
	draw_wall_stripe(&render->frame, col, tex, render->ray.column);
}
