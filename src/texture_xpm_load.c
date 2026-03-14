#include "cub3d.h"
#include "cub_config.h"
#include <stdlib.h>

// 何する関数か:
// - テクスチャスロットを未ロード状態へ戻し、画像メタ情報を 0 初期化する。
// 参照でいじった値:
// - `wall_texture->loaded` を `false` に戻す。
// - `wall_texture->image.img`, `addr` を `NULL` に戻す。
// - `wall_texture->image.bpp`, `line_len`, `endian`, `width`, `height`
//   を `0` に戻す。
// 戻り値の意味:
// - なし。
static void	reset_texture_slot(t_texture *wall_texture)
{
	wall_texture->loaded = false;
	wall_texture->image.img = NULL;
	wall_texture->image.addr = NULL;
	wall_texture->image.bpp = 0;
	wall_texture->image.line_len = 0;
	wall_texture->image.endian = 0;
	wall_texture->image.width = 0;
	wall_texture->image.height = 0;
}

// 何する関数か:
// - 既存の MLX 画像があれば破棄し、テクスチャスロットを空状態へ戻す。
// 参照でいじった値:
// - `wall_texture->image.img` を必要なら `mlx_destroy_image()` で破棄する。
// - `wall_texture` 全体を未ロードの空状態へ戻す。
// 戻り値の意味:
// - なし。
static void	destroy_texture_slot(void *mlx_instance, t_texture *wall_texture)
{
	if (wall_texture->image.img != NULL)
		mlx_destroy_image(mlx_instance, wall_texture->image.img);
	reset_texture_slot(wall_texture);
}

// 何する関数か:
// - XPM ファイル 1 枚を MLX に読み込み、pixel 書き込み情報を取得する。
// - 途中で失敗したら、画像を破棄してスロットを空状態に戻す。
// 参照でいじった値:
// - `wall_texture->image.img` に画像本体を入れる。
// - `wall_texture->image.addr`, `bpp`, `line_len`, `endian` を設定する。
// - `wall_texture->image.width`, `height` を画像サイズで設定する。
// - `wall_texture->loaded` をロード成功時に `true` にする。
// 戻り値の意味:
// - `true`: XPM 読み込みと pixel 情報の取得に成功した。
// - `false`: 引数不正または途中の MLX 呼び出しに失敗した。
static bool	load_wall_texture(t_texture *wall_texture, void *mlx_instance,char *texture_path){
	char	*pixel_addr;

	if (wall_texture == NULL || mlx_instance == NULL || texture_path == NULL)
		return false;
	destroy_texture_slot(mlx_instance, wall_texture);
	wall_texture->image.img = mlx_xpm_file_to_image(mlx_instance,texture_path, &wall_texture->image.width,&wall_texture->image.height);
	if (wall_texture->image.img == NULL)
		return false;
	pixel_addr = mlx_get_data_addr(wall_texture->image.img,&wall_texture->image.bpp, &wall_texture->image.line_len,&wall_texture->image.endian);
	if (pixel_addr == NULL){
		destroy_texture_slot(mlx_instance, wall_texture);
		return false;
	}
	wall_texture->image.addr = pixel_addr;
	wall_texture->loaded = true;
	return true;
}

// 何する関数か:
// - 四方向の壁テクスチャを順に読み込み、`assets->wall[]` に設定する。
// 参照でいじった値:
// - `assets->wall[]` の各スロットに画像本体と pixel 情報を設定する。
// - 読み込みに失敗したスロットは空状態のままにする。
// 戻り値の意味:
// - `true`: 必要な壁テクスチャをすべて読み込めた。
// - `false`: 引数不正またはどれか 1 枚でも読み込みに失敗した。
bool	texture_xpm_load_all(t_assets *assets, t_tex_path texture_paths,t_mlx mlx_context){
	int	texture_index;

	if (assets == NULL || mlx_context.mlx == NULL)
		return false;
	texture_index = 0;
	while (texture_index < TEX_COUNT){
		if (load_wall_texture(&assets->wall[texture_index],
				mlx_context.mlx, texture_paths.path[texture_index]) == false)
			return false;
		++texture_index;
	}
	return true;
}
