/**
 * @file ray_casting.h
 * @brief レイキャスト・描画・プレイヤー更新・ゲームループの関数宣言
 *
 * @details
 * このモジュール群は,毎フレームの更新と描画の中心を担う.
 * `update_player()`で入力からプレイヤー状態を更新し,
 * `cast_ray()`で各画面列の壁衝突を求め,
 * `render_column()`と`render_frame()`でフレームバッファへ変換する.
 * 数学的には,2Dベクトル演算,回転行列,DDA,テクスチャ座標補間が主役になる.
 */
#ifndef RAY_CASTING_H
#define RAY_CASTING_H

#include "cub3d.h"

/**
 * @brief 1フレーム更新処理の結果を表す列挙体
 */
typedef enum e_game_tick_status
{
	GAME_TICK_CONTINUE = 0,	/**< 正常に1フレーム処理した */
	GAME_TICK_STOP,			/**< 終了要求を受け取った */
	GAME_TICK_ERROR			/**< 異常終了扱いの失敗が起きた */
}	t_game_tick_status;

void	cast_ray(t_ray *ray, const t_player *player, const t_map *map,
			int col);
void	render_column(t_render *render, const t_assets *assets);
void	render_frame(t_game *game);
void	rotate_player(t_player *player, double angle);
void	update_player(t_player *player, const t_input *input,
			const t_map *map, double dt);
t_game_tick_status	game_loop_tick(t_game *game);

#endif
