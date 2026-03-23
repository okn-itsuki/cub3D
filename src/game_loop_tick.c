#include "ray_casting.h"
#include "game_init.h"
#include <sys/time.h>
#include <stdlib.h>

#define US_PER_SEC 1000000
#define SEC_PER_US 1e-6

static uint64_t	get_current_time_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((uint64_t)tv.tv_sec * US_PER_SEC + (uint64_t)tv.tv_usec);
}
