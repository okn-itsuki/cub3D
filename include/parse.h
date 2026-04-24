#ifndef PARSE_H
#define PARSE_H

#include <sys/types.h>
#include <stdbool.h>
#include "parse_line_type.h"
#include "parse_excepion.h"
#include "cub_config.h"

#define BUF_SIZE 4096
#define TEX_ID_LEN 2

t_excepion			parse_cub(const char *path, t_config *config);
void				init_config(t_config *config);
void				destroy_config(t_config *config);

// read_file.c
t_excepion			read_file_lines(const char *path, char ***ptr);
void				free_lines(char **lines);

// split_lines.c
t_excepion			split_lines(const char *content, char ***lines);

// system_err_mes.c
t_excepion			open_file(const char *path, int *fd);
t_excepion			read_file_buf(int fd, char *buf, size_t buf_size, ssize_t *read_size);
t_excepion			close_file(int fd);
t_excepion			malloc_err(void);
t_excepion			excepion_argment(const char *message);
t_excepion			excepion_texture(const char *message);
t_excepion			excepion_color(const char *message);
t_excepion			excepion_message(const char *message, t_excepion state);
t_excepion			excepion_map(const char *message);

// parse_utils.c
const char			*parse_skip_spaces(const char *str);
bool				parse_is_blank_line(const char *line);
bool				parse_extension(const char *filename, const char *extension);
t_parse_line_type	parse_get_line_type(const char *line);

// parse_elements.c
t_excepion			parse_header_line(const char *line, t_config *config);
t_excepion			parse_all_headers_set(const t_config *config);
t_excepion			parse_texture_line(const char *line, t_config *config);

// parse_color.c
t_excepion			parse_color_line(const char *line, t_config *config);

// parse_map.c
t_excepion			parse_map_lines(char **lines, int start, int end, t_config *config);

#endif
