#include "parse_test.h"
#include "cub_config.h"
#include <stdio.h>

// 概要 : t_config 全体をまとめて表示する
// - manko
// - chinko
void	print_config(const t_config *config)
{
	printf("=== t_config ===\n");
	print_tex_paths(&config->tex);
	printf("\n");
	print_colors(&config->floor_color, &config->ceiling_color);
	printf("\n");
	print_map(&config->map);
	printf("\n");
	print_spawn(&config->spawn);
}
