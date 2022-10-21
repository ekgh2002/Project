/*
 * TIME_CLOCK.c
 *
 * Created: 2022-08-12 오후 2:51:57
 *  Author: kccistc
 */ 


#include "TIME_CLOCK.h"   // "" 는 현재 dire에 있는 화일을 물고 온다.


int watch_state=0;
uint32_t watchclock=0;
int watchclock_hour=0;
int watchclock_min=0;
int watchclock_sec=0;

TIME time_clock = {0,0,0};

void get_time_clock(TIME *time)
{
	*time = time_clock;
}

void set_time_clock(TIME time)
{
	time_clock = time;
}

// 1초에 1번씩 call을 한다. 분초
void inc_time_sec(void)
{
	time_clock.sec++;
	if (time_clock.sec >= 60)
	{
		time_clock.sec=0;
		time_clock.min++;
		if (time_clock.min >= 60)
		{
			time_clock.min=0;
			time_clock.hour++;
			if (time_clock.hour >= 24)
			{
				time_clock.hour=0;
			}
		}
	}
	
}