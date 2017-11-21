#ifdef _WIN32
#include <time.h>
#else
#include <sys/time.h>
#endif

double wall_time(void)
{
#ifdef _WIN32
  return (double)((double)clock() / (double)CLOCKS_PER_SEC);
#else
  struct timeval tv;
  struct timezone tz;
  static long t0=0;
  static long t0usec=0;

  gettimeofday(&tv, &tz);
  if (t0==0) {
    t0=tv.tv_sec;
    t0usec=tv.tv_usec;
  }
  return (tv.tv_sec-t0 + (tv.tv_usec-t0usec)/1000000.0);
#endif
}
