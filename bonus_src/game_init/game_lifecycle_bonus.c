/**
 * @file game_lifecycle_bonus.c
 * @brief `t_game`全体の初期化と破棄
 */
#include "game_init_bonus.h"
#include "libft.h"

/**
 * @brief `t_game`全体を初期状態へ初期化する
 *
 * 生のゼロクリアだけでは意味を持たないフィールドについては,
 * `init_config()`と`init_input()`を通して明示的に初期値を与える.
 *
 * @param[out] game 初期化対象のゲーム状態
 */
void	init_game(t_game *game)
{
	if (game == NULL)
		return ;
	ft_bzero(game, sizeof(*game));
	init_config(&game->config);
	init_input(&game->input);
}

/**
 * @brief `t_game`全体を破棄し,再利用できる初期状態へ戻す
 *
 * 実行時資源を片付けた後にconfigも破棄し,最後に`init_game()`で
 * 再初期化することで再利用可能な空状態を作る.
 *
 * @param[in,out] game 破棄対象のゲーム状態
 */
void	destroy_game(t_game *game)
{
	if (game == NULL)
		return ;
	destroy_game_resources(game);
	destroy_config(&game->config);
	init_game(game);
}
