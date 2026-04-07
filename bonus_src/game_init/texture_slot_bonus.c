/**
 * @file texture_slot_bonus.c
 * @brief 壁テクスチャ1スロット分のロード・破棄処理
 */
#include "game_init/game_init_internal_bonus.h"
#include "libft.h"

/**
 * @brief 画像メタ情報をゼロ初期化する
 */
static void	reset_image(t_img *image)
{
	ft_bzero(image, sizeof(*image));
}

/**
 * @brief テクスチャスロットを未ロード状態にリセットする
 *
 * @param[out] texture リセット対象のテクスチャスロット
 */
void	reset_texture_slot(t_texture *texture)
{
	if (texture == NULL)
		return ;
	reset_image(&texture->image);
}

/**
 * @brief テクスチャスロットのMLX画像を破棄し,空状態に戻す
 *
 * @param[in]     mlx_instance MLXインスタンス
 * @param[in,out] texture      破棄対象のテクスチャスロット
 */
void	destroy_texture_slot(void *mlx_instance, t_texture *texture)
{
	if (texture == NULL)
		return ;
	if (mlx_instance != NULL && texture->image.img != NULL)
		mlx_destroy_image(mlx_instance, texture->image.img);
	reset_texture_slot(texture);
}

/**
 * @brief XPMファイル1枚をMLXに読み込み,ピクセル情報を取得する
 *
 * 途中で失敗した場合,画像を破棄してスロットを空状態に戻す。
 *
 * @param[in,out] texture      読み込み先のテクスチャスロット
 * @param[in]     mlx_instance MLXインスタンス
 * @param[in]     texture_path XPMファイルのパス
 * @retval true  XPM読み込みとピクセル情報の取得に成功
 * @retval false 引数不正またはMLX呼び出しに失敗
 */
bool	load_texture_slot(t_texture *texture, void *mlx_instance,
		char *texture_path)
{
	if (texture == NULL || mlx_instance == NULL || texture_path == NULL)
		return (false);
	destroy_texture_slot(mlx_instance, texture);
	texture->image.img = mlx_xpm_file_to_image(mlx_instance, texture_path,
			&texture->image.width, &texture->image.height);
	if (texture->image.img == NULL)
		return (false);
	texture->image.addr = mlx_get_data_addr(texture->image.img,
			&texture->image.bpp, &texture->image.line_len,
			&texture->image.endian);
	if (texture->image.addr != NULL)
		return (true);
	destroy_texture_slot(mlx_instance, texture);
	return (false);
}
