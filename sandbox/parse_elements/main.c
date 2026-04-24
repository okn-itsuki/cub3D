/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 00:00:00 by codex             #+#    #+#             */
/*   Updated: 2026/04/17 00:00:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "parse.h"

static void	print_textures(const t_config *config);
static void	print_colors(const t_config *config);
static int	run_lines(int ac, char **av, t_config *config);

static void	print_textures(const t_config *config)
{
	printf("NO: %s\n", config->tex.path[TEX_NO]);
	printf("SO: %s\n", config->tex.path[TEX_SO]);
	printf("WE: %s\n", config->tex.path[TEX_WE]);
	printf("EA: %s\n", config->tex.path[TEX_EA]);
}

static void	print_colors(const t_config *config)
{
	printf("F: %d,%d,%d (%u)\n", config->floor_color.r,
		config->floor_color.g, config->floor_color.b,
		config->floor_color.value);
	printf("C: %d,%d,%d (%u)\n", config->ceiling_color.r,
		config->ceiling_color.g, config->ceiling_color.b,
		config->ceiling_color.value);
}

static int	run_lines(int ac, char **av, t_config *config)
{
	int	index;

	index = 1;
	while (index < ac)
	{
		printf("parse: %s\n", av[index]);
		if (!parse_header_line(av[index], config))
			return (1);
		index++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_config	config;

	if (ac < 2)
	{
		printf("usage: ./parse_elements \"NO ./n.xpm\" \"F 1,2,3\"\n");
		return (1);
	}
	init_config(&config);
	if (run_lines(ac, av, &config) != 0)
	{
		destroy_config(&config);
		return (1);
	}
	print_textures(&config);
	print_colors(&config);
	printf("all_headers_set: %s\n",
		(parse_all_headers_set(&config) ? "true" : "false"));
	destroy_config(&config);
	return (0);
}
