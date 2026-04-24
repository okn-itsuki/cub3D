#ifndef PARSE_EXCEPION_H
#define PARSE_EXCEPION_H

typedef enum s_excepion
{
	SUCCESS,
	CLOSE_ERR,
	OVFL_ERR,
	OPEN_ERR,
	READ_ERR,
	MALLOC_FAIL,
	INVALID_STRING,
	TEX_ERR,
	COLOR_ERR
} t_excepion;

#endif