/**
 * @file texture_assets_bonus.c
 * @brief 壁テクスチャ群のロード・破棄処理
 */
#include "game_init/game_init_internal_bonus.h"

/**
 * @brief テクスチャIDに対応する所有ビットを返す
 */
static uint32_t	texture_flag(t_tex_id tex_id)
{
	return (1u << tex_id);
}

/**
 * @brief 四方向の壁テクスチャを順にロードする
 *
 * 途中で1枚でも失敗した場合,ロード済みの画像もまとめて破棄する。
 *
 * @param[in,out] assets        テクスチャの格納先
 * @param[in]     texture_paths 四方向のXPMファイルパス
 * @param[in]     mlx_context   MLXコンテキスト
 * @retval true  全テクスチャのロードに成功
 * @retval false いずれかのロードに失敗
 */
static bool	load_wall_textures(t_assets *assets, t_tex_path texture_paths,
		t_mlx mlx_context)
{
	int			index;
	uint32_t	flag;

	index = 0;
	while (index < TEX_COUNT)
	{
		flag = texture_flag((t_tex_id)index);
		if (!load_texture_slot(&assets->wall[index], mlx_context.mlx,
				texture_paths.path[index]))
		{
			destroy_texture_assets(assets, mlx_context);
			return (false);
		}
		assets->wall_mask |= flag;
		++index;
	}
	return (true);
}

/**
 * @brief ロード済みの壁テクスチャをすべて破棄する
 *
 * wall_maskが立っているスロットだけを対象に破棄し,空状態へ戻す。
 *
 * @param[in,out] assets      テクスチャアセット
 * @param[in]     mlx_context MLXコンテキスト
 */
void	destroy_texture_assets(t_assets *assets, t_mlx mlx_context)
{
	int			index;
	uint32_t	flag;

	if (assets == NULL)
		return ;
	index = 0;
	while (index < TEX_COUNT)
	{
		flag = texture_flag((t_tex_id)index);
		if ((assets->wall_mask & flag) != 0u)
			destroy_texture_slot(mlx_context.mlx, &assets->wall[index]);
		++index;
	}
	assets->wall_mask = 0u;
}

/**
 * @brief ゲーム状態の壁テクスチャをロードし,runtime_maskを更新する
 *
 * @param[in,out] game          テクスチャを格納するゲーム状態
 * @param[in]     texture_paths 四方向のXPMファイルパス
 * @retval true  壁テクスチャのロードが最後まで成功
 * @retval false ロード途中で失敗
 */
bool	init_game_wall_textures(t_game *game, t_tex_path texture_paths)
{
	if (game == NULL || game->mlx.mlx == NULL)
		return (false);
	game->runtime_mask &= ~GAME_WALL_TEXTURES_READY;
	destroy_texture_assets(&game->assets, game->mlx);
	if (!load_wall_textures(&game->assets, texture_paths, game->mlx))
		return (false);
	game->runtime_mask |= GAME_WALL_TEXTURES_READY;
	return (true);
}
