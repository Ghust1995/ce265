#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

float wall_time(void);

// inCircle: generate "points" random points at domain
//           (xStart, yStart) : (xStart+xRange, yStart+yRange), 
//           that is a part of the square (-1.,-1):(1,1) and
//           count how many of these points lye inside the
//           unit circle

long inCircle(long points,   // # random pairs to generate
	      float xStart,  // start point at x domain
	      float xRange,  // range of x domain
	      float yStart,  // start point at y domain
	      float yRange)  // range of y domain
{
  long count;
  long inside;
  int randVal;
  float xRand;
  float yRand;

  // random point is (xStart,yStart) + random fraction
  // of (xRange,yRange);
  // inside unit circle iff distance to origin <= 1.0

  inside=0;
  for (count=0; count<points; count++) {
    randVal=rand();
    xRand = xStart + xRange*(float)randVal/(float)RAND_MAX;
    randVal=rand();
    yRand = yStart + yRange*(float)randVal/(float)RAND_MAX;
    if (xRand*xRand + yRand*yRand <= 1.0) inside++;
  }
  return inside;
}
      
int main(int argc, char *argv[]) {
  int procs;
  int pow;
  float xStart, xRange;
  float yStart, yRange;
  long points;
  long inside;
  float tend, tstart;
  double computedPi, referencePi, relError;

  // input arguments

  if (argc != 2) {
    printf(" use: <exec> <exponent> (range [0:30]) \n");
    return -1;
  }
  pow = atoi(argv[1]);
  if (pow < 0 || pow > 30) {
    printf(" exponent (%d) outside range [0:30] \n", pow);
    return -1;
  }
  procs=1;
  points = 1L << pow;

  // square that circumscribes unit circle

  xStart=-1.0; xRange=2.0;
  yStart=-1.0; yRange=2.0;

  // compute pi and measure execution time

  tstart = wall_time();
  inside = inCircle(points, 
		    xStart, xRange,
		    yStart, yRange);
  computedPi = 4.0* ((double)inside/(double)points);
  tend = wall_time() - tstart;

  // relative error and final dump

  referencePi = 2.0*asin(1.0);
  relError = 100.0*fabs(computedPi-referencePi)/referencePi;
  printf("pi=%9.7f, rel error %8.4f%%, %d procs, 2^%d points, %f(s) exec time\n", 
	 computedPi, relError, procs, pow, tend);

  return 0;
}
