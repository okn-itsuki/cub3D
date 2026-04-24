#ifndef PARSE_SYSTEM_ERR_H
#define PARSE_SYSTEM_ERR_H

typedef enum s_sytem_err
{
	SUCCESS,
	CLOSE_ERR,
	OVFL_ERR,
	OPEN_ERR,
	READ_ERR,
	MALLOC_ERR,
	INVALID_STRING,
	TEX_ERR,
	COLOR_ERR,
	MAP_ERR
}	t_system_err;

typedef t_system_err	t_excepion;

#endif