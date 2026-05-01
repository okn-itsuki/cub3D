/**
 * @file parse_map.c
 * @brief マップ本体の複製と妥当性検証
 */
#include <stdlib.h>
#include "libft.h"
#include "parse.h"

static t_excepion	copy_map_lines(char **lines, int start, int end,
						t_config *config);
static t_excepion	replace_spawn(char *line, int row, t_config *config);
static t_excepion	validate_map(const t_config *config);
static bool			row_is_closed(const t_map *map, int row);
static bool			is_valid_map_char(char c);

/**
 * @brief マップ行列を `t_config.map` と `spawn` 情報へ変換します。
 *
 * @param lines `.cub` 全体の行配列。
 * @param start マップ開始行の index。
 * @param end マップ終了直後の index。
 * @param config 解析結果の格納先。
 *
 * @retval SUCCESS マップ複製と検証の両方に成功した場合。
 * @retval MAP_ERR マップ欠落、文字不正、プレイヤー不正、閉塞不正時。
 * @retval MALLOC_ERR メモリ確保失敗時。
 */
t_excepion	parse_map_lines(char **lines, int start, int end, t_config *config)
{
	t_excepion	state;

	if (start >= end)
		return (excepion_map("map is missing\n"));
	state = copy_map_lines(lines, start, end, config);
	if (state != SUCCESS)
		return (state);
	return (validate_map(config));
}

/**
 * @brief マップ行をヒープへ複製し、最大幅とスポーン情報を確定します。
 *
 * @param lines 元の行配列。
 * @param start マップ開始 index。
 * @param end マップ終了直後の index。
 * @param config マップ情報の保存先。
 *
 * @retval SUCCESS すべての行の複製に成功した場合。
 * @retval MAP_ERR 行内容不正があった場合。
 * @retval MALLOC_ERR メモリ確保失敗時。
 */
static t_excepion	copy_map_lines(char **lines, int start, int end,
	t_config *config)
{
	t_excepion	state;
	int			row;
	size_t		width;

	config->map.height = end - start;
	config->map.grid = ft_calloc(config->map.height + 1, sizeof(char *));
	if (config->map.grid == NULL)
		return (malloc_err());
	row = 0;
	config->map.width = 0;
	while (start + row < end)
	{
		config->map.grid[row] = ft_strdup(lines[start + row]);
		if (config->map.grid[row] == NULL)
			return (malloc_err());
		state = replace_spawn(config->map.grid[row], row, config);
		if (state != SUCCESS)
			return (state);
		width = ft_strlen(config->map.grid[row]);
		if ((int)width > config->map.width)
			config->map.width = (int)width;
		++row;
	}
	config->map.grid[row] = NULL;
	return (SUCCESS);
}

/**
 * @brief 1 行中のプレイヤー記号を `spawn` に反映し、床セルへ正規化します。
 *
 * @param line 検査・書き換え対象のマップ行。
 * @param row マップ上の行番号。
 * @param config スポーン情報の保存先。
 *
 * @retval SUCCESS 行内文字が妥当で、プレイヤー数も条件を満たす場合。
 * @retval MAP_ERR 禁止文字またはプレイヤー複数配置を検出した場合。
 */
static t_excepion	replace_spawn(char *line, int row, t_config *config)
{
	int	col;

	col = 0;
	while (line[col] != '\0')
	{
		if (!is_valid_map_char(line[col]))
			return (excepion_map("map contains invalid character\n"));
		if (ft_strchr("NSEW", line[col]) != NULL)
		{
			if (config->spawn.dir != DIR_UNSET)
				return (excepion_map("map must contain exactly one player\n"));
			config->spawn.row = row;
			config->spawn.col = col;
			if (line[col] == 'N')
				config->spawn.dir = NORTH;
			else if (line[col] == 'S')
				config->spawn.dir = SOUTH;
			else if (line[col] == 'E')
				config->spawn.dir = EAST;
			else
				config->spawn.dir = WEST;
			line[col] = '0';
		}
		++col;
	}
	return (SUCCESS);
}

/**
 * @brief 複製済みマップのサイズ・プレイヤー・外周閉塞を検証します。
 *
 * @param config 検証対象の設定構造体。
 *
 * @retval SUCCESS マップが実行可能な形で閉じている場合。
 * @retval MAP_ERR サイズ不足、プレイヤー不足、または壁の閉塞不正がある場合。
 */
static t_excepion	validate_map(const t_config *config)
{
	int		row;
	int		col;
	char	cell;

	if (config->map.height < 3 || config->map.width < 3)
		return (excepion_map("map is too small\n"));
	if (config->spawn.dir == DIR_UNSET)
		return (excepion_map("map must contain exactly one player\n"));
	row = 0;
	while (row < config->map.height)
	{
		if (!row_is_closed(&config->map, row))
			return (excepion_map("map must be closed by walls\n"));
		if (row == 0 || row == config->map.height - 1)
		{
			col = 0;
			while (col < config->map.width)
			{
				cell = map_cell_at(&config->map, col, row);
				if (cell != ' ' && cell != '1')
					return (excepion_map("map must be closed by walls\n"));
				++col;
			}
		}
		++row;
	}
	return (SUCCESS);
}

/**
 * @brief 指定行の左右端が壁で閉じているか判定します。
 *
 * @retval true 最初と最後の非空白セルがともに `1` の場合。
 * @retval false 行が空、または左右端が壁でない場合。
 */
static bool	row_is_closed(const t_map *map, int row)
{
	int		left;
	int		right;

	left = 0;
	while (left < map->width && map_cell_at(map, left, row) == ' ')
		++left;
	if (left == map->width)
		return (false);
	right = map->width - 1;
	while (right >= 0 && map_cell_at(map, right, row) == ' ')
		--right;
	if (map_cell_at(map, left, row) != '1')
		return (false);
	if (map_cell_at(map, right, row) != '1')
		return (false);
	return (true);
}

/**
 * @brief マップ内で許可される文字か判定します。
 *
 * @retval true `0` `1` 空白 `N` `S` `E` `W` のいずれかの場合。
 * @retval false 上記以外の文字の場合。
 */
static bool	is_valid_map_char(char c)
{
	return (c == '0' || c == '1' || c == ' '
		|| c == 'N' || c == 'S' || c == 'E' || c == 'W');
}
