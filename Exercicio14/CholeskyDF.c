#include "wall_time.h"
#include "BasicBlock.h"
#include "CholeskyDF.h"

//Driver para Cholesky paralelo heroico

void CholeskyDF(ptrBM A, double *timeDF) {
  volatile int **core_progress;
  int k, m, nBK;
  double t0;

  nBK=A->n;
  core_progress = (volatile int **) calloc(nBK, sizeof(int*));
#pragma omp parallel for private(k,m)
   for (k=0; k<nBK; k++) {
    core_progress[k] = (int *) calloc(nBK, sizeof(int));
    for (m=0; m<nBK; m++) 
      core_progress[k][m]=0;
  }
  
  t0 = wall_time();
#pragma omp parallel
  {
    int threadID=omp_get_thread_num();
    int nThreads=omp_get_max_threads();
    CholeskyParBM(A, nBK, threadID, nThreads, core_progress);
  }
  *timeDF = wall_time()-t0;

  free(core_progress);
}

// Cholesky paralelo heorico por blocos
// decompoe MatrizSimetrica(A) em L e L^t 
// retorna L na triangular inferior de A


void CholeskyParBM(ptrBM A, int nBK, int threadID, int nThreads, 
		   int volatile **core_progress) {
  int k, m, n;
  int next_k, next_m, next_n;
  
  k=0; m=threadID;
  while (m>=nBK) {
    k++; m=m-nBK+k;
  } n=0;
  
  while (k<nBK && m<nBK) {
    next_n=n; next_m=m; next_k=k;
    
    next_n++; 
    if (next_n > next_k) {
      next_m += nThreads;
      while (next_m >= nBK && next_k < nBK) {
	next_k++; next_m=next_m-nBK+next_k;
      } next_n=0;
    }
    
    if (m==k) {
      if (n==k) {
	POTRF(A,k,k); // A[k][k] <- cholesky (A[k][k])
	core_progress[k][k]=1;
      }
      else {
	while (core_progress[k][n] != 1);
	SYRK(A,k,k, A,k,n, -1.0); // A[k][k] <- A[k][k] - A[k][n] x A[k][n]^T
      }
    }
    else {
      if (n==k) {
	while (core_progress[k][k] != 1);
	TRSM(A,m,k, A,k,k); // A[m][k] <- A[m][k] x A[k][k]^(-1T)
	core_progress[m][k]=1;
      }
      else {
	while (core_progress[k][n] != 1);
	while (core_progress[m][n] != 1);
	GEMM(A,m,k, A,m,n, A,k,n, -1.0); // A[m][k] <- A[m][k] - A[m][n] x A[k][n]^T
      }
    }
    n=next_n; m=next_m; k=next_k;
  }
}
