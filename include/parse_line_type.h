#ifndef PARSE_LINE_TYPE_H
#define PARSE_LINE_TYPE_H


/**
 * @file parse_line_type.h
 * @brief その行がどの要素の行なのか

 * @param LINE_EMPTY NULL
 * @param LINE_TEXTURE NO, SO, WE, EA
 * @param LINE_COLOR F or C
 * @param LINE_MAP 1 0 N 
 * @param LINE_INVALID 無効な値
 */
typedef enum e_parse_line_type
{
	LINE_EMPTY,
	LINE_TEXTURE,
	LINE_COLOR,
	LINE_MAP,
	LINE_INVALID
}	t_parse_line_type;

#endif