#include "cub3d.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_config	config;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <map.cub>\n", argv[0]);
		return (1);
	}
	// TODO: parse_cub(argv[1], &config) が実装されたらここで呼ぶ
	(void)config;
	printf("file: %s\n", argv[1]);
	return (0);
}
