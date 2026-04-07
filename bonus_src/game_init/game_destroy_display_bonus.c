/**
 * @file game_destroy_display_bonus.c
 * @brief MLX表示コンテキストの破棄
 *
 * @details
 * MLXはプラットフォーム差異があるため,表示コンテキストの後始末を
 * 1か所へ閉じ込める.これにより,呼び出し側は「MLX本体を破棄する」
 * という抽象責務だけを意識すればよく,OS分岐を他所へ漏らさない.
 */
#include "game_init/game_init_internal_bonus.h"
#include <stdlib.h>

/**
 * @brief MLX表示コンテキストをプラットフォームに応じて破棄する
 *
 * Linuxでは`mlx_destroy_display()`を呼んだあと,確保済みインスタンスをfreeする.
 * macOS版MLXでは`free()`のみで後始末する.
 * 呼び出し側はこの差異を知らなくてよいようにしてあり,
 * `destroy_game_resources()`は常にこの関数を経由してMLX本体を閉じる.
 *
 * @param[in] mlx_instance 破棄対象のMLXインスタンス
 */
void	destroy_mlx_display(void *mlx_instance)
{
	if (mlx_instance == NULL)
		return ;
#ifdef __linux__
	mlx_destroy_display(mlx_instance);
#endif
	free(mlx_instance);
}
