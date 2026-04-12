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

#endif
