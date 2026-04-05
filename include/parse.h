#ifndef PARSE_H
#define PARSE_H

#include <stdbool.h>
#include "cub_config.h"

bool parse_cub(const char *path, t_config *config);
void free_config(t_config *config);

// read_file.c
char **read_file_lines(const char *path);
void free_lines(char **lines);

// split_lines.c
char **split_lines(char *content);

#endif
