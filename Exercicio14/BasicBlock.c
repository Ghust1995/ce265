#include "BasicBlock.h"


// aloca matriz por blocos com nxn blocos de tamanho bxb;
// inicializa a matriz para 0.0


ptrBM CreateFullBM(int b, int n) {
  ptrBM this;
  float *thisBlock;
  int ib, jb, i, j;
  
  this = (ptrBM) calloc(1, sizeof(BM));
  this->n = n;
  this->b = b;
  this->M = (float **) calloc(n*n, sizeof(float*));
  for (ib=0; ib<n; ib++) {
    for (jb=0; jb<n; jb++) {
      this->M[in(ib,jb,n)] = (float *) calloc(b*b, sizeof(float));
    }
  }
  for (ib=0; ib<n; ib++) {
    for (jb=0; jb<n; jb++) {
      thisBlock = this->M[in(ib,jb,n)];
      for (i=0; i<b; i++) {
	for (j=0; j<b; j++) {
	  thisBlock[in(i,j,b)]=0.0;
	}
      }
    }
  }
  
  return(this);
}


// inicializa matriz triangular inferior com diagonal dominante


void SetLowerBM(ptrBM this) {
  float *thisBlock;
  float den;
  int b, n, ib, jb, i, j, N;
  
  n=this->n;
  b=this->b;
  N=n*b;
  den = 1.0/(float) (RAND_MAX);
  
  for (ib=0; ib<n; ib++) {
    for (jb=0; jb<ib; jb++) {
      thisBlock = this->M[in(ib,jb,n)];
      for (i=0; i<b; i++) {
	for (j=0; j<b; j++) {
	  thisBlock[in(i,j,b)]=den * (float) rand();
	}
      }
    }
    thisBlock = this->M[in(ib,ib,n)];
    for (i=0; i<b; i++) {
      for (j=0; j<i; j++) {
	thisBlock[in(i,j,b)]=den * (float) rand();
      }
      thisBlock[in(i,i,b)]=den * (float) rand() + (float) N;
    }
  }
}


// inicializa A = LxL^T, dado L triangular inferior


void GenPosDefBM(ptrBM L, ptrBM A) {
  int n, ib, jb, kb;
  float alfa=1.0;
  
  n=L->n;
  for (ib=0; ib<n; ib++) {
    for (jb=0; jb<=ib; jb++) {
      for (kb=0; kb<=jb; kb++) {
	GEMM(A, ib, jb,
	     L, ib, kb, 
	     L, jb, kb,
	     alfa);
      }
    }
  }
}


// dump matriz por blocos


void DumpMatrixBM(ptrBM this) {
  float *thisBlock;
  int b, n, it, jt, ib, jb, i, j;
  
  n=this->n;
  b=this->b;
  printf("Block Matrix with %d x %d blocks of size %d x %d\n",
	 n,n,b,b);
  for (it=0; it<n*b; it++) {
    ib=it/b;
    i=it%b;
    for (jt=0; jt<n*b; jt++) {
      jb=jt/b;
      j=jt%b;
      thisBlock = this->M[in(ib,jb,n)];
      printf("%f ",thisBlock[in(i,j,b)]);
    }
    printf("\n");
  }
}


// verifica erros absoluto e relativo em A=LxL^T


void VerifyBM(ptrBM Res, ptrBM Ori) {
  float *thisRes, *thisOri;
  float maxAbs=0.0;
  float maxAbsEntry1=0.0;
  float maxAbsEntry2=0.0;
  float maxRel=0.0;
  float maxRelEntry1=0.0;
  float maxRelEntry2=0.0;
  int b, n, ib, jb, i, j;
  float errAbs, errRel;
  
  n=Res->n;
  b=Res->b;
  for (ib=0; ib<n; ib++) {
    for (jb=0; jb<ib; jb++) {
      thisRes = Res->M[in(ib,jb,n)];
      thisOri = Ori->M[in(ib,jb,n)];
      for (i=0; i<b; i++) {
	for (j=0; j<b; j++) {
	  errAbs = fabsf(thisRes[in(i,j,b)] - thisOri[in(i,j,b)]);
	  if (errAbs > maxAbs) {
	    maxAbs=errAbs;
	    maxAbsEntry1=thisRes[in(i,j,b)];
	    maxAbsEntry2=thisOri[in(i,j,b)];
	  }
	  if (thisOri[in(i,j,b)] != 0.0) {
	    errRel=errAbs/thisOri[in(i,j,b)];
	    if (errRel > maxRel) {
	      maxRel=errRel;
	      maxRelEntry1=thisRes[in(i,j,b)];
	      maxRelEntry2=thisOri[in(i,j,b)];
	    }
	  }
	}
      }
    }
    jb=ib;
    thisRes = Res->M[in(ib,jb,n)];
    thisOri = Ori->M[in(ib,jb,n)];
    for (i=0; i<b; i++) {
      for (j=0; j<=i; j++) {
	errAbs = fabsf(thisRes[in(i,j,b)] - thisOri[in(i,j,b)]);
	if (errAbs > maxAbs) {
	  maxAbs=errAbs;
	  maxAbsEntry1=thisRes[in(i,j,b)];
	  maxAbsEntry2=thisOri[in(i,j,b)];
	}
	if (thisOri[in(i,j,b)] != 0.0) {
	  errRel=errAbs/thisOri[in(i,j,b)];
	  if (errRel > maxRel) {
	    maxRel=errRel;
	    maxRelEntry1=thisRes[in(i,j,b)];
	    maxRelEntry2=thisOri[in(i,j,b)];
	  }
	}
      }
    }
  }
  printf("Maximo Erro Absoluto=%f%c, MatrizA=%f, MatrizL=%f \n",
	 maxAbs, ' ', maxAbsEntry1, maxAbsEntry2);
  printf("Maximo Erro Relativo=%f%c, MatrizA=%f, MatrizL=%f \n",
	 maxRel, '%', maxRelEntry1, maxRelEntry2);
}


// blocoCompleto(C) = blocoCompleto(C) + alfa*blocoCompleto(A)*blocoCompleto(B)^T


void GEMM(ptrBM C, int ibC, int jbC,
	  ptrBM A, int ibA, int jbA, 
	  ptrBM B, int ibB, int jbB,
	  float alfa) {
  float *blkA, *blkB, *blkC;
  int b, n, i, j, k;
  float aux;
  
  n=A->n;
  b=A->b;
  blkA = A->M[in(ibA,jbA,n)];
  blkB = B->M[in(ibB,jbB,n)];
  blkC = C->M[in(ibC,jbC,n)];
  
  for (i=0; i<b; i++) {
    for (j=0; j<b; j++) {
      aux = 0.0;
      for (k=0; k<b; k++) 
	aux += blkA[in(i,k,b)]*blkB[in(j,k,b)];
      blkC[in(i,j,b)]=blkC[in(i,j,b)]+alfa*aux;
    }
  }
}


// decompoe blocoSimetrico(A) em L e L^T
// retorna L na triangular inferior de A


void POTRF(ptrBM A, int ibA, int jbA) {
  float *blkA;
  int b, nBK, k, n, m;
  
  nBK=A->n;
  b=A->b;
  blkA = A->M[in(ibA,jbA,nBK)];
  
  for (k=0; k<b; k++) {
    for (n=0; n<k; n++) {
      blkA[in(k,k,b)] -= (blkA[in(k,n,b)]*blkA[in(k,n,b)]);
    }
    blkA[in(k,k,b)]=sqrtf(blkA[in(k,k,b)]);
    for (m=k+1; m<b; m++) {
      for (n=0; n<k; n++) {
	blkA[in(m,k,b)] -= (blkA[in(m,n,b)]*blkA[in(k,n,b)]);
      }
      blkA[in(m,k,b)]= blkA[in(m,k,b)]/blkA[in(k,k,b)];
    }
  }
}


// blocoSimetrico(A) = blocoSimetrico(A) + alfa*blocoCompleto(B)*blocoCompleto(B)^T


void SYRK(ptrBM A, int ibA, int jbA, 
	  ptrBM B, int ibB, int jbB,
	  float alfa) {
  float *blkA, *blkB;
  int b, n, i, j, k;
  float aux;
  
  n=A->n;
  b=A->b;
  blkA = A->M[in(ibA,jbA,n)];
  blkB = B->M[in(ibB,jbB,n)];
  
  for (i=0; i<b; i++) {
    for (j=0; j<=i; j++) {
      aux = 0.0;
      for (k=0; k<b; k++) 
	aux += blkB[in(i,k,b)]*blkB[in(j,k,b)];
      blkA[in(i,j,b)]+=alfa*aux;
    }
  }
}


// obtem bloco(X)  tal que
// blocoCompleto(X)*blocoTriangularInferior(B)^T = blocoCompleto(A)
// retorna X no bloco A


void TRSM(ptrBM A, int ibA, int jbA, 
	  ptrBM B, int ibB, int jbB) {
  float *blkA, *blkB;
  int b, n, i, j, k;
  float aux;
  
  n=A->n;
  b=A->b;
  blkA = A->M[in(ibA,jbA,n)];
  blkB = B->M[in(ibB,jbB,n)];
  
  for (i=0; i<b; i++) {
    for (j=0; j<b; j++) {
      aux = 0.0;
      for (k=0; k<j; k++) 
	aux += blkA[in(i,k,b)]*blkB[in(j,k,b)];
      blkA[in(i,j,b)]=(blkA[in(i,j,b)]-aux)/blkB[in(j,j,b)];
    }
  }
}
