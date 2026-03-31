#include "game_init/game_init_internal.h"

static uint32_t	texture_flag(t_tex_id tex_id)
{
	return (1u << tex_id);
}

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
