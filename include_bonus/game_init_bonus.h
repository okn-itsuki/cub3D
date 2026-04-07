/**
 * @file game_init_bonus.h
 * @brief ゲーム資源の初期化・破棄・入力ハンドラの宣言
 *
 * @details
 * このモジュールは,ゲーム起動時に必要な資源の生成と破棄,
 * およびイベントフックの接続を担当する.
 * MLX本体,ウィンドウ,フレームバッファ,壁テクスチャの所有状態は
 * `t_game.runtime_mask`で追跡され,途中失敗時でも逆順に安全に片付けられる.
 */
#ifndef BONUS_GAME_INIT_H
# define BONUS_GAME_INIT_H

# include "cub3d_bonus.h"

/**
 * @brief ゲーム実行時資源の所有状態を管理するビットマスク
 *
 * t_game.runtime_maskの各ビットで、どの資源まで初期化済みかを追跡する。
 * 破棄時はこのマスクを参照し、所有している資源だけを安全に解放する。
 */
typedef enum e_game_runtime_mask
{
	GAME_MLX_READY = 1u << 0,			/**< MLX本体が初期化済み */
	GAME_WINDOW_READY = 1u << 1,		/**< ウィンドウが作成済み */
	GAME_FRAME_READY = 1u << 2,			/**< フレームバッファが作成済み */
	GAME_WALL_TEXTURES_READY = 1u << 3	/**< 壁テクスチャがロード済み */
}	t_game_runtime_mask;

void	init_config(t_config *config);
void	init_game(t_game *game);
bool	init_game_mlx(t_game *game_state);
bool	init_game_wall_textures(t_game *game_state, t_tex_path texture_paths);
void	init_player(t_player *player, t_spawn spawn);
void	init_input(t_input *input);
void	destroy_config(t_config *config);
void	destroy_texture_assets(t_assets *assets, t_mlx mlx_context);
void	destroy_game_resources(t_game *game_state);
void	destroy_game(t_game *game);
int		handle_key_press(int keycode, t_game *game);
int		handle_key_release(int keycode, t_game *game);
int		handle_close(t_game *game);
void	register_hooks(t_game *game);

#endif
