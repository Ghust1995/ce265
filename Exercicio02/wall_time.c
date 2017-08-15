#include <time.h>
#include <stdio.h>

double wall_time(void)
{
#ifdef _WIN32
  return (double)((double)clock() / (double)CLOCKS_PER_SEC);
#else
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv, &tz);
  return(tv.tv_sec + tv.tv_usec/1000000.0);
#endif
}
