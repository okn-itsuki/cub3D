#include "parse_test.h"
#include "cub_config.h"
#include <stdio.h>

static const char	*g_tex_label[TEX_COUNT] = {
	[TEX_NO] = "NO",
	[TEX_SO] = "SO",
	[TEX_WE] = "WE",
	[TEX_EA] = "EA",
};

// 概要 : テクスチャパス (NO/SO/WE/EA) を表示する
void	print_tex_paths(const t_tex_path *tex)
{
	int	i;

	printf("[tex_path]\n");
	i = 0;
	while (i < TEX_COUNT)
	{
		if (tex->path[i])
			printf("  %s: %s\n", g_tex_label[i], tex->path[i]);
		else
			printf("  %s: (null)\n", g_tex_label[i]);
		i++;
	}
}
