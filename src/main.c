/**
 * @file main.c
 * @brief cub3Dのエントリポイント
 */
#include "cub3d.h"
#include <stdio.h>
#include "game_config.h"
#include "game_init.h"
#include "ray_casting.h"
#include "stdlib.h"
#include "parse.h"

static void shutdown_game(t_game *game,int exit_code){
	destroy_game(game);
	exit(exit_code);
}

static int game_hook(t_game *game)
{
	t_game_tick_status status ;

	status =  game_loop_tick(game);
	if(status == GAME_TICK_ERROR )
		shutdown_game(game,EXIT_FAILURE);
	if(status == GAME_TICK_STOP)
		shutdown_game(game,EXIT_SUCCESS);
	
	return 0;
}

bool game_render(t_game *game)
{
	bool is_ok;


	is_ok = init_game_mlx(game);
	if(is_ok)
		is_ok = init_game_wall_textures(game,game->config.tex);
	if(is_ok)
		init_player(&game->player,game->config.spawn);
	if(!is_ok){
		destroy_game(game);
		return false;
	}
	game->running = true;
	register_hooks(game);
	mlx_loop_hook(game->mlx.mlx,game_hook,game);
	mlx_loop(game->mlx.mlx);
	return true;

}


bool cub3d(t_game *game)
{
	if(game_render(game) == false)
		return false; 


	return true;
}



/**
 * @brief プログラムのエントリポイント
 *
 * コマンドライン引数から.cubファイルを受け取り、ゲームを起動する。
 *
 * @param[in] argc 引数の数 (2であること)
 * @param[in] argv argv[1]に.cubファイルのパスを指定
 * @return 正常終了時0、引数エラー時1
 */
int	main(int argc, char **argv)
{
	t_game game;
	t_excepion	state;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <map.cub>\n", argv[0]);
// TODO : iokuno
// t_excepion に INVALID_ARG_COUNT を追加する。
		// return (INVALID_ARG_COUNT);
	}
	init_game(&game); 
	state = parse_cub(argv[1], &game.config);
	if (state != SUCCESS)
		return 1;
	if(cub3d(&game) == false)
		return 1;
//TODO : render()
	return (state);
}
