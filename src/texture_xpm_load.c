#include "cub3d.h"
#include "cub_config.h"
#include <stdlib.h>



static bool	texture_xpm_load_one(t_assets *assets, t_tex_path path_dir, t_mlx mlx, t_tex_id id)
{
	t_texture	*tex;

	if (assets == NULL || mlx.mlx == NULL)
		return false;
	if (id < 0 || id >= TEX_COUNT)
		return false;
	if (path_dir.path[id] == NULL)
		return false;
	tex = &assets->wall[id];
	tex->loaded = false;
	tex->image.img = mlx_xpm_file_to_image(mlx.mlx,path_dir.path[id],&tex->image.width,&tex->image.height);
	if (tex->image.img == NULL)
		return false;
	tex->image.addr = mlx_get_data_addr(tex->image.img,&tex->image.bpp,&tex->image.line_len,&tex->image.endian);
	if (tex->image.addr == NULL)
		return false;
	tex->loaded = true;
	return true;
}


bool texture_xpm_load_all(t_assets *assets,t_tex_path path_dir,t_mlx mlx,t_tex_id id){

	if(texture_xpm_load_one(assets,path_dir,mlx,TEX_NO) == false)
		return false;
	if(texture_xpm_load_one(assets,path_dir,mlx,TEX_SO) == false)
		return false;
	if(texture_xpm_load_one(assets,path_dir,mlx,TEX_WE) == false)
		return false;
	if(texture_xpm_load_one(assets,path_dir,mlx,TEX_EA) == false)
		return false;
}