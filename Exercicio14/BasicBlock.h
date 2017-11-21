#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#define in(i,j,dj) i*dj+j
double wall_time(void);
	   
typedef struct {
  int n;      // number of block rows (or columns)
  int b;      // block size
  float **M; // array 2D of blocks
} BM, *ptrBM;

// aloca matriz por blocos com nxn blocos de tamanho bxb
// elementos da matriz inicializados para 0.0
ptrBM CreateFullBM(int b, int n);

// gera elementos da matriz triangular inferior com diagonal dominante
void SetLowerBM(ptrBM this);

// preenche A = LxL^T, com L triangular inferior
void GenPosDefBM(ptrBM L, ptrBM A);

// dump matriz por blocos
void DumpMatrixBM(ptrBM this);

// verifica erros absoluto e relativo em A=LxL^T
void VerifyBM(ptrBM, ptrBM);

// blocoCompleto(C) = blocoCompleto(C) + alfa*blocoCompleto(A)*blocoCompleto(B)^T
void GEMM(ptrBM C, int ibC, int jbC,
	  ptrBM A, int ibA, int jbA, 
	  ptrBM B, int ibB, int jbB,
	  float alfa);

// decompoe blocoSimetrico(A) em L e L^t 
// ou seja, cholesky em um bloco; 
// retorna L na triangular inferior de A
void POTRF(ptrBM A, int ibA, int jbA);

// blocoSimetrico(A) = blocoSimetrico(A) + alfa*blocoCompleto(B)*blocoCompleto(B)^T
void SYRK(ptrBM A, int ibA, int jbA, 
	  ptrBM B, int ibB, int jbB,
	  float alfa);

// obtem bloco(X)  tal que
// blocoCompleto(A) = blocoCompleto(X)*blocoTriangularInferior(B)^T
// retorna X no bloco A
void TRSM(ptrBM A, int ibA, int jbA, 
	  ptrBM B, int ibB, int jbB);


// Cholesky paralelo por blocos
// decompoe MatrizSimetrica(A) em L e L^t 
// retorna L na triangular inferior de A
//void CholeskyParBM(ptrBM A, int nBK, int threadID, int nThreads, 
//		   int volatile **core_progress);
