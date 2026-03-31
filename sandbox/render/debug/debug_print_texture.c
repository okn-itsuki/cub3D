#include "cub3d.h"
#include <stdio.h>

static void	debug_print_texture_image_one(const t_texture *tex)
{
	int	loaded;

	if (tex == NULL)
		return ;
	loaded = (tex->image.img != NULL);
	printf("loaded=%d img=%p w=%d h=%d addr=%p bpp=%d line=%d endian=%d\n",
		loaded,
		(void *)tex->image.img,
		tex->image.width,
		tex->image.height,
		(void *)tex->image.addr,
		tex->image.bpp,
		tex->image.line_len,
		tex->image.endian);
}

void	debug_print_texture_image_all(const t_assets *assets)
{
	debug_print_texture_image_one(&assets->wall[TEX_NO]);
	debug_print_texture_image_one(&assets->wall[TEX_SO]);
	debug_print_texture_image_one(&assets->wall[TEX_WE]);
	debug_print_texture_image_one(&assets->wall[TEX_EA]);
}
