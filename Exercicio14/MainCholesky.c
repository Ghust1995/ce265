#include "BasicBlock.h"
#include "CholeskyOMP.h"
#include "CholeskyDF.h"

int main(int argc, char **argv) {
  int nBK=10;
  int b=900;
  int size=nBK*b;
  int nThreads;
  ptrBM L, A;
  double timeOMP, timeDF, dSize, flops;
  int k, m;

  dSize = (double) size;
  flops = dSize*dSize*dSize/3.0;
  nThreads=omp_get_max_threads();
  printf("Cholesky paralelo por blocos com %d threads em matriz %ld x %ld com %d x %d blocos de %d x %d elementos\n",
	 nThreads,size,size,nBK,nBK,b,b);

  L = CreateFullBM(b,nBK);
  SetLowerBM(L);

  A = CreateFullBM(b,nBK);
  GenPosDefBM(L, A);

  CholeskyOMP(A, &timeOMP);
  printf("threads=%d; tempoOMP=%f; GFlopsOMP=%f\n", 
	 omp_get_max_threads(), timeOMP, flops/(1.0e9*timeOMP));

  GenPosDefBM(L, A);

  CholeskyDF(A, &timeDF);
  printf("threads=%d; tempoDF =%f; GFlopsDF =%f\n", 
	 omp_get_max_threads(), timeDF, flops/(1.0e9*timeDF));

  exit(0);
}
