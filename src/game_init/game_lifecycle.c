#include "game_init.h"
#include "libft.h"

void	init_game(t_game *game)
{
	if (game == NULL)
		return ;
	ft_bzero(game, sizeof(*game));
	init_config(&game->config);
	init_input(&game->input);
}

void	destroy_game(t_game *game)
{
	if (game == NULL)
		return ;
	destroy_game_resources(game);
	destroy_config(&game->config);
	init_game(game);
}
