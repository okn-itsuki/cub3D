#ifndef DEBUG_DASHBOARD_H
# define DEBUG_DASHBOARD_H

# ifdef DEBUG_DASHBOARD

#  ifdef BONUS_MODE
#   include "cub3d_bonus.h"
#  else
#   include "cub3d.h"
#  endif

typedef struct s_fps_stats
{
	double	current;
	double	average;
	double	min;
	double	max;
	double	sum;
	int		count;
}	t_fps_stats;

typedef struct s_dashboard
{
	int			term_rows;
	int			term_cols;
	t_fps_stats	fps;
	uint64_t	prev_time_us;
	int			enabled;
}	t_dashboard;

void	dashboard_init(t_dashboard *dash);
void	dashboard_update(t_dashboard *dash, const t_game *game);
void	dashboard_cleanup(void);

# endif
#endif
