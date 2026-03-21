#include "ray_casting.h"
#include "mlx.h"

// 何する関数か:
// - 1フレームを描画する。全列をraycastし、列単位で天井/壁/床を描画後、windowに転送する。
// 参照でいじった値:
// - `game->render.frame` の全ピクセルを書き換える。
// - `game->render.ray`, `game->render.column` を各列で上書きする。
// 戻り値の意味:
// - なし。
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
