#ifndef PARSE_H
#define PARSE_H

#include <stdbool.h>
#include "parse_system_err.h"
#include "cub_config.h"

bool			parse_cub(const char *path, t_config *config);
void			free_config(t_config *config);

// read_file.c
t_system_err	read_file_lines(const char *path, char ***ptr);
void			free_lines(char **lines);

// split_lines.c
t_system_err	split_lines(const char *content, char ***lines);

// system_err_mes.c
t_system_err	open_file(const char *path, int *fd);
t_system_err	read_file_buf(int fd, char *buf, size_t buf_size, ssize_t *read_size);
t_system_err	close_file(int fd);
t_system_err	malloc_err(void);
t_excepion		excepion_map(const char *message);
t_excepion		excepion_color(const char *message);

// parse_utils.c
const char		*parse_skip_spaces(const char *str);
bool			parse_is_blank_line(const char *line);

// parse_map.c
t_excepion		parse_map_lines(char **lines, int start, int end,
					t_config *config);

#endif
