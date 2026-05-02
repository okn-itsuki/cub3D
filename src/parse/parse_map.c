/**
 * @file parse_map.c
 * @brief マップ本体の複製と妥当性検証
 */
#include <stdlib.h>
#include "libft.h"
#include "parse.h"

typedef struct s_pt
{
	int	x;
	int	y;
}	t_pt;

static t_excepion	copy_map_lines(char **lines, int start, int end,
						t_config *config);
static t_excepion	replace_spawn(char *line, int row, t_config *config);
static t_excepion	validate_map(const t_config *config);
static bool			**alloc_visited(int height, int width);
static void			free_visited(bool **vis, int height);
static t_excepion	check_nb(t_pt nb, const t_config *cfg, bool **vis,
						t_pt *q, int *tail);
static t_excepion	bfs(const t_config *cfg, bool **vis, t_pt *q);
static t_excepion	flood_fill(const t_config *config);
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
 * @retval MALLOC_FAIL メモリ確保失敗時。
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
 * @retval MALLOC_FAIL メモリ確保失敗時。
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
 * @brief `height × width` の `bool` 二次元配列をゼロ初期化で確保します。
 *
 * @return 確保した配列。失敗時は `NULL`。
 */
static bool	**alloc_visited(int height, int width)
{
	bool	**vis;
	int		row;

	vis = ft_calloc(height, sizeof(bool *));
	if (vis == NULL)
		return (NULL);
	row = 0;
	while (row < height)
	{
		vis[row] = ft_calloc(width, sizeof(bool));
		if (vis[row] == NULL)
		{
			while (--row >= 0)
				free(vis[row]);
			free(vis);
			return (NULL);
		}
		++row;
	}
	return (vis);
}

/**
 * @brief `alloc_visited` で確保した配列を解放します。
 */
static void	free_visited(bool **vis, int height)
{
	int	row;

	row = 0;
	while (row < height)
		free(vis[row++]);
	free(vis);
}

static t_excepion	check_nb(t_pt nb, const t_config *cfg, bool **vis,
	t_pt *q, int *tail)
{
	if (nb.x < 0 || nb.y < 0 || nb.x >= cfg->map.width
		|| nb.y >= cfg->map.height)
		return (excepion_map("map must be closed by walls\n"));
	if (map_cell_at(&cfg->map, nb.x, nb.y) == '0' && !vis[nb.y][nb.x])
	{
		vis[nb.y][nb.x] = true;
		q[(*tail)++] = nb;
	}
	return (SUCCESS);
}

static t_excepion	bfs(const t_config *cfg, bool **vis, t_pt *q)
{
	t_pt		qt;
	t_pt		cur;
	t_excepion	state;
	int			i;

	qt.x = 0;
	qt.y = 0;
	vis[cfg->spawn.row][cfg->spawn.col] = true;
	q[qt.y++] = (t_pt){cfg->spawn.col, cfg->spawn.row};
	while (qt.x < qt.y)
	{
		cur = q[qt.x++];
		i = -1;
		while (++i < 4)
		{
			state = check_nb((t_pt){cur.x + (i == 2) - (i == 3),
					cur.y + (i == 0) - (i == 1)}, cfg, vis, q, &qt.y);
			if (state != SUCCESS)
				return (state);
		}
	}
	return (SUCCESS);
}

/**
 * @brief spawn からの BFS でプレイヤーが範囲外へ出られないか検証します。
 *
 * @retval SUCCESS プレイヤーが閉じた領域内に留まる場合。
 * @retval MAP_ERR 範囲外へ到達可能な経路が存在する場合。
 * @retval MALLOC_FAIL メモリ確保失敗時。
 */
static t_excepion	flood_fill(const t_config *config)
{
	bool		**vis;
	t_pt		*queue;
	t_excepion	state;

	vis = alloc_visited(config->map.height, config->map.width);
	if (vis == NULL)
		return (malloc_err());
	queue = ft_calloc(config->map.height * config->map.width, sizeof(t_pt));
	if (queue == NULL)
		return (free_visited(vis, config->map.height), malloc_err());
	state = bfs(config, vis, queue);
	free(queue);
	free_visited(vis, config->map.height);
	return (state);
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
	if (config->map.height < 3 || config->map.width < 3)
		return (excepion_map("map is too small\n"));
	if (config->spawn.dir == DIR_UNSET)
		return (excepion_map("map must contain exactly one player\n"));
	return (flood_fill(config));
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
