#include "parse.h"
#include "parse_test.h"
#include <stdio.h>

int	main(int ac, char **av)
{
	t_config	config;

	if (ac != 2)
	{
		fprintf(stderr, "Usage: %s <map.cub>\n", av[0]);
		return (1);
	}
	if (parse_cub(av[1], &config) != SUCCESS)
		return (1);
	print_config(&config);
	destroy_config(&config);
	return (0);
}
