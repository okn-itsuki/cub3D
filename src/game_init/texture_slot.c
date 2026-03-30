#include "game_init/game_init_internal.h"
#include "libft.h"

static void	reset_image(t_img *image)
{
	ft_bzero(image, sizeof(*image));
}

void	reset_texture_slot(t_texture *texture)
{
	if (texture == NULL)
		return ;
	reset_image(&texture->image);
}

void	destroy_texture_slot(void *mlx_instance, t_texture *texture)
{
	if (texture == NULL)
		return ;
	if (mlx_instance != NULL && texture->image.img != NULL)
		mlx_destroy_image(mlx_instance, texture->image.img);
	reset_texture_slot(texture);
}

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
