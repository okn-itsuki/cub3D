/**
 * @file parse_color.c
 * @brief F/C 色要素を解析して t_config に格納する
 */
#include "libft.h"
#include "parse.h"

static t_excepion	parse_rgb(const char *value, t_rgb *rgb);
static t_excepion	parse_rgb_values(const char **value, int *components);
static bool			parse_rgb_component(const char **value, int *out);

/**
 * @brief F または C の色指定行を解析する。
 *
 * 先頭の空白を飛ばしたあと、識別子に応じて床色または天井色へ
 * 格納する。対象の色がすでに設定済みならエラーを返す。
 *
 * @param line F または C で始まる設定行
 * @param config 解析結果の格納先
 *
 * @retval SUCCESS 色が正常に解析できた場合
 * @retval COLOR_ERR 色識別子の重複または RGB 形式が不正な場合
 */
t_excepion	parse_color_line(const char *line, t_config *config)
{
	t_rgb		*target;
	t_excepion	state;

	line = parse_skip_spaces(line);
	target = &config->floor_color;
	if (*line == 'C')
		target = &config->ceiling_color;
	if (target->is_set)
		return (excepion_color("duplicate color identifier"));
	state = parse_rgb(line + 1, target);
	if (state != SUCCESS)
		return (state);
	return (SUCCESS);
}

/**
 * @brief RGB 文字列を t_rgb に変換する。
 *
 * 先頭の空白を許容し、3 つの色成分を読み取ったあとに余分な文字が
 * 残っていないことを確認する。成功時は packed color と is_set も更新
 * する。
 *
 * @param value 色識別子の後続文字列
 * @param rgb 解析結果の格納先
 *
 * @retval SUCCESS RGB を正常に格納できた場合
 * @retval COLOR_ERR 成分数不足、範囲外、余剰文字がある場合
 */
static t_excepion	parse_rgb(const char *value, t_rgb *rgb)
{
	int			components[3];
	t_excepion	state;

	value = parse_skip_spaces(value);
	state = parse_rgb_values(&value, components);
	if (state != SUCCESS)
		return (state);
	value = parse_skip_spaces(value);
	if (*value != '\0')
		return (excepion_color("rgb contains extra characters\n"));
	rgb->r = components[0];
	rgb->g = components[1];
	rgb->b = components[2];
	rgb->value = ((uint32_t)rgb->r << 16)
		| ((uint32_t)rgb->g << 8) | (uint32_t)rgb->b;
	rgb->is_set = true;
	return (SUCCESS);
}

/**
 * @brief RGB 3 成分を順番に読み取る。
 *
 * 各成分の前後にある空白は許容する。1, 2 成分目の直後にはカンマが
 * 必須で、3 成分ちょうど読み取れない場合はエラーを返す。
 *
 * @param value 入力位置へのポインタ
 * @param components 読み取った 3 成分の格納先
 *
 * @retval SUCCESS 3 成分を正しい書式で読み取れた場合
 * @retval COLOR_ERR 数値不正または区切りが不正な場合
 */
static t_excepion	parse_rgb_values(const char **value, int *components)
{
	int	index;

	index = 0;
	while (index < 3)
	{
		if (!parse_rgb_component(value, &components[index]))
			return (excepion_color("invalid rgb value\n"));
		*value = parse_skip_spaces(*value);
		if (index < 2 && **value != ',')
			return (excepion_color("rgb must contain 3 components\n"));
		if (index < 2)
			*value = parse_skip_spaces(*value + 1);
		index++;
	}
	return (SUCCESS);
}

/**
 * @brief RGB 1 成分を 10 進数として読み取る。
 *
 * 先頭が数字であることを確認し、連続する数字列を 0 から 255 の範囲で
 * 整数へ変換する。成功時は value を読み取り後の位置まで進める。
 *
 * @param value 入力位置へのポインタ
 * @param out 読み取った成分値の格納先
 *
 * @retval true 成分を正常に読み取れた場合
 * @retval false 数字がない、または 255 を超えた場合
 */
static bool	parse_rgb_component(const char **value, int *out)
{
	int	result;

	if (!ft_isdigit((unsigned char)**value))
		return (false);
	result = 0;
	while (ft_isdigit((unsigned char)**value))
	{
		result = result * 10 + (**value - '0');
		if (result > 255)
			return (false);
		(*value)++;
	}
	*out = result;
	return (true);
}

static t_excepion	parse_rgb_component(const char **value, int *out)
{
	int result;
	t_excepion state;

	if (!ft_isdigit((unsigned char)**value))
		return (excepiom_message("Color code must be numeric.", CLOSE_ERR));	
	result = 0;
	while (ft_isdigit(unsigned char)**value)
	{
		result = result * 10 + (**value - '0');
		if (result > 255)
			return (excepiom_message("Color code must be 0-255.", CLOSE_ERR));
		(*value)++;
	}
	*out = result;
	return (SUCCESS);
}
