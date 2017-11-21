#include "wall_time.h"
#include "BasicBlock.h"
#include "CholeskyOMP.h"


// Cholesky por blocos paralelizado OpenMP
// decompoe MatrizSimetrica(A) em L e L^t 
// retorna L na triangular inferior de A


void CholeskyOMP(ptrBM A, double *timeOMP){
  int k, n, m, nBK;
  double t0;
  
  t0 = wall_time();
  nBK=A->n;
#pragma omp parallel private(k,n,m)
  {
    for (k=0; k<nBK; k++) {
#pragma omp single
      {
	for (n=0; n<k; n++) {
	  SYRK(A,k,k, A,k,n, -1.0); // A[k,k] = A[k,k]-A[k,n]A[k,n]^T 
	}
	POTRF(A,k,k);  // A[k,k] = Decomposicao A[k,k] em LL^T 
      }
#pragma omp for
      for (m=k+1; m<nBK; m++) {
	for (n=0; n<k; n++) {
	  GEMM(A,m,k, A,m,n, A,k,n, -1.0); // A[m,k] = A[m,k]-A[m,n]A[k,n]^T
	}
	TRSM(A,m,k, A,k,k);  // A[m,k] = A[m,k](A[k,k]^T)^-1
      }
    }
  }
  *timeOMP = wall_time()-t0;
}
