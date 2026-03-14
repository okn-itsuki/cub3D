#include "cub3d.h"
#include "cub_config.h"
#include <stdlib.h>

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

static void	destroy_texture_slot(void *mlx_instance, t_texture *wall_texture)
{
	if (wall_texture->image.img != NULL)
		mlx_destroy_image(mlx_instance, wall_texture->image.img);
	reset_texture_slot(wall_texture);
}

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


bool	texture_xpm_load_all(t_assets *assets, t_tex_path texture_paths,
		t_mlx mlx_context)
{
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
