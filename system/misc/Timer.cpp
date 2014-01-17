//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "Timer.h"
#ifdef _WIN32
   #include <mmsystem.h>
#else
//   fixme
#endif
//---------------------------------------------------------------------------


#ifdef _WIN32
Timer::Timer() {
   timeBeginPeriod(1);
   lastTime = timeGetTime();
}

card32 Timer::getCurrentValue() {
   return timeGetTime();
}

card32 Timer::getDeltaValue(card32 lastValue) {
   return timeGetTime() - lastValue;
}

card32 Timer::getDeltaValue() {
   card32 t = timeGetTime();
   card32 result = t - lastTime;
   lastTime = t;
   return result;
}

Timer::~Timer() {
   timeEndPeriod(1);
}
#else
#include <time.h>

/* WORKAROUND FOR MAC OS X - MISSING CLOCK_GETTIME */
#ifdef __APPLE__
#include <mach/mach_time.h>
#include <sys/types.h>
#include <sys/time.h>

#define CLOCK_MONOTONIC 0

int clock_gettime(int unused, struct timespec *tp) {
	static mach_timebase_info_data_t mtid;
	static struct timeval tv;
	static uint64_t first_mat;
	uint64_t elapsed_ns;

	if(!mtid.denom) {
		mach_timebase_info(&mtid);
		gettimeofday(&tv, NULL);
		first_mat = mach_absolute_time();
	}

	elapsed_ns = (mach_absolute_time() - first_mat) * mtid.numer / mtid.denom;
	tp->tv_sec = tv.tv_sec + (long)elapsed_ns / 1000000000;
	tp->tv_nsec = tv.tv_usec*1000 + elapsed_ns % 1000000000;
	if(tp->tv_nsec >= 1000000000) {
		tp->tv_sec++;
		tp->tv_nsec -= 1000000000;
	}

	return 0;
}
#endif

Timer::Timer()
{
	lastTime = getCurrentValue();
}

Timer::~Timer() {
}

card32 Timer::getCurrentValue()
{
	timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec * 1000 + t.tv_nsec / 1000000;
}

card32 Timer::getDeltaValue(card32 lastValue)
{
	return getCurrentValue() - lastValue;
}

card32 Timer::getDeltaValue()
{
	card32 t = getCurrentValue();
	card32 result = t - lastTime;
	lastTime = t;
	return result;
}
#endif
