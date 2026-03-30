/**
 * @file main.c
 * @brief cub3Dのエントリポイント
 */
#include "cub3d.h"
#include <stdio.h>

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
	t_config	config;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <map.cub>\n", argv[0]);
		return (1);
	}
	/* TODO: parse_cub(argv[1], &config) が実装されたらここで呼ぶ */
	(void)config;
	printf("file: %s\n", argv[1]);
	return (0);
}
