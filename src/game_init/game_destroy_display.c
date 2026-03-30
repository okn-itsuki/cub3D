#include "game_init/game_init_internal.h"
#include <stdlib.h>

void	destroy_mlx_display(void *mlx_instance)
{
	if (mlx_instance == NULL)
		return ;
#ifdef __linux__
	mlx_destroy_display(mlx_instance);
#endif
	free(mlx_instance);
}
