/**
 * @file map_access_bonus.c
 * @brief 不揃いなマップ行でも安全に使える参照関数
 */
#include "cub_config_bonus.h"

/**
 * @brief 1行文字列から指定列のセルを安全に取り出す
 *
 * @param[in] row 行文字列
 * @param[in] x   列番号
 * @return 指定セル文字. 存在しなければ`' '`
 */
static char	row_cell_at(const char *row, int x)
{
	int	index;

	if (row == NULL || x < 0)
		return (' ');
	index = 0;
	while (index < x && row[index] != '\0')
		++index;
	if (index != x || row[index] == '\0')
			return (' ');
	return (row[index]);
}

/**
 * @brief マップ上の1セルを安全に参照する
 *
 * @param[in] map 参照対象のマップ
 * @param[in] x   列番号
 * @param[in] y   行番号
 * @return 指定セル文字. 範囲外や欠損は`' '`
 */
char	map_cell_at(const t_map *map, int x, int y)
{
	if (map == NULL || y < 0 || y >= map->height)
		return (' ');
	return (row_cell_at(map->grid[y], x));
}

/**
 * @brief 指定セルが移動不能かどうかを返す
 *
 * @param[in] map 判定対象のマップ
 * @param[in] x   列番号
 * @param[in] y   行番号
 * @retval true  壁またはvoid
 * @retval false 通行可能
 */
bool	map_is_solid(const t_map *map, int x, int y)
{
	char	cell;

	cell = map_cell_at(map, x, y);
	return (cell == '1' || cell == ' ');
}
