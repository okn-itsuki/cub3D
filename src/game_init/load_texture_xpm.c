/**
 * @file load_texture_xpm.c
 * @brief 壁テクスチャ(XPM)のロード・破棄処理
 */
#include "game_init.h"
#include <stdlib.h>

/**
 * @brief テクスチャスロットを未ロード状態にリセットする
 *
 * 画像ポインタをNULL、メタ情報を0、loadedをfalseに戻す。
 *
 * @param[out] wall_texture リセット対象のテクスチャスロット
 */
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

/**
 * @brief テクスチャスロットのMLX画像を破棄し、空状態に戻す
 *
 * @param[in]     mlx_instance MLXインスタンス
 * @param[in,out] wall_texture 破棄対象のテクスチャスロット
 */
static void	destroy_texture_slot(void *mlx_instance, t_texture *wall_texture)
{
	if (wall_texture->image.img != NULL)
		mlx_destroy_image(mlx_instance, wall_texture->image.img);
	reset_texture_slot(wall_texture);
}

/**
 * @brief ロード済みの壁テクスチャをすべて破棄する
 *
 * loaded == true のスロットだけを対象に破棄し、空状態へ戻す。
 *
 * @param[in,out] assets      テクスチャアセット (NULLなら何もしない)
 * @param[in]     mlx_context MLXコンテキスト (mlxがNULLなら何もしない)
 */
void	destroy_texture_assets(t_assets *assets, t_mlx mlx_context){
	int	texture_index;

	if (assets == NULL || mlx_context.mlx == NULL)
		return ;
	texture_index = 0;
	while (texture_index < TEX_COUNT){
		if (assets->wall[texture_index].loaded)
			destroy_texture_slot(mlx_context.mlx, &assets->wall[texture_index]);
		++texture_index;
	}
}

/**
 * @brief XPMファイル1枚をMLXに読み込み、ピクセル情報を取得する
 *
 * 途中で失敗した場合、画像を破棄してスロットを空状態に戻す。
 *
 * @param[in,out] wall_texture 読み込み先のテクスチャスロット
 * @param[in]     mlx_instance MLXインスタンス
 * @param[in]     texture_path XPMファイルのパス
 * @retval true  XPM読み込みとピクセル情報の取得に成功
 * @retval false 引数不正またはMLX呼び出しに失敗
 */
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

/**
 * @brief 四方向の壁テクスチャを順にロードする
 *
 * 途中で1枚でも失敗した場合、ロード済みの画像もまとめて破棄する。
 *
 * @param[in,out] assets        テクスチャの格納先
 * @param[in]     texture_paths 四方向のXPMファイルパス
 * @param[in]     mlx_context   MLXコンテキスト
 * @retval true  全テクスチャのロードに成功
 * @retval false 引数不正またはいずれかのロードに失敗
 */
static bool	load_all_wall_textures(t_assets *assets, t_tex_path texture_paths,t_mlx mlx_context){
	int	texture_index;

	if (assets == NULL || mlx_context.mlx == NULL)
		return false;
	texture_index = 0;
	while (texture_index < TEX_COUNT){
		if (load_wall_texture(&assets->wall[texture_index],
				mlx_context.mlx, texture_paths.path[texture_index]) == false){
			destroy_texture_assets(assets, mlx_context);
			return false;
		}
		++texture_index;
	}
	return true;
}

/**
 * @brief ゲーム状態の壁テクスチャをロードし、init_maskを更新する
 *
 * @param[in,out] game_state    テクスチャを格納するゲーム状態
 * @param[in]     texture_paths 四方向のXPMファイルパス
 * @retval true  壁テクスチャのロードが最後まで成功
 * @retval false 引数不正またはロード途中で失敗
 */
bool	init_game_wall_textures(t_game *game_state, t_tex_path texture_paths){
	if (game_state == NULL || game_state->mlx.mlx == NULL)
		return false;
	game_state->init_mask &= ~GAME_WALL_TEXTURES_READY;
	destroy_texture_assets(&game_state->assets, game_state->mlx);
	if (load_all_wall_textures(&game_state->assets, texture_paths,
			game_state->mlx) == false)
		return false;
	game_state->init_mask |= GAME_WALL_TEXTURES_READY;
	return true;
}
