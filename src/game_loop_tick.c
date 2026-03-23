#include "ray_casting.h"
#include "game_init.h"
#include <sys/time.h>
#include <stdlib.h>

static uint64_t	get_current_time_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((uint64_t)tv.tv_sec * 1000000 + (uint64_t)tv.tv_usec);
}