#ifndef PARSE_H
#define PARSE_H

#include <stdbool.h>
#include "cub_config.h"

typedef enum s_sytem_err
{
	SUCCESS,
	CLOSE_ERR,
	OVFL_ERR,
	OPEN_ERR,
	READ_ERR,
	MALLOC_ERR
}t_system_err;

bool			parse_cub(const char *path, t_config *config);
void			free_config(t_config *config);

// read_file.c
t_system_err	read_file_lines(const char *path, char ***ptr);
void			free_lines(char **lines);

// split_lines.c
t_system_err	split_lines(char *content, char ***lines);

#endif
