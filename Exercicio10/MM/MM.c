#include <stdio.h>
#include <stdlib.h>
#define ind2d(i,j) i*tam+j

double wall_time(void);


void MMijk(int tam, float* matA, float* matB, float* matC) {
  int i, j, k;
  for (i=0; i<tam; i++)
    for (j=0; j<tam; j++) 
      for (k=0; k<tam; k++)
	matC[ind2d(i,j)] = matC[ind2d(i,j)] + matA[ind2d(i,k)]*matB[ind2d(k,j)];
}


void MMikj(int tam, float* matA, float* matB, float* matC) {
  int i, j, k;
  for (i=0; i<tam; i++)
    for (k=0; k<tam; k++)
      for (j=0; j<tam; j++) 
	matC[ind2d(i,j)] = matC[ind2d(i,j)] + matA[ind2d(i,k)]*matB[ind2d(k,j)];
}

void MMkij(int tam, float* matA, float* matB, float* matC) {
  int i, j, k;
  for (k=0; k<tam; k++)
    for (i=0; i<tam; i++)
      for (j=0; j<tam; j++) 
	matC[ind2d(i,j)] = matC[ind2d(i,j)] + matA[ind2d(i,k)]*matB[ind2d(k,j)];
}

void MMkji(int tam, float* matA, float* matB, float* matC) {
  int i, j, k;
  for (k=0; k<tam; k++)
    for (j=0; j<tam; j++) 
      for (i=0; i<tam; i++)
	matC[ind2d(i,j)] = matC[ind2d(i,j)] + matA[ind2d(i,k)]*matB[ind2d(k,j)];
}

void MMjik(int tam, float* matA, float* matB, float* matC) {
  int i, j, k;
  for (j=0; j<tam; j++) 
    for (i=0; i<tam; i++)
      for (k=0; k<tam; k++)
	matC[ind2d(i,j)] = matC[ind2d(i,j)] + matA[ind2d(i,k)]*matB[ind2d(k,j)];
}

void MMjki(int tam, float* matA, float* matB, float* matC) {
  int i, j, k;
  for (j=0; j<tam; j++) 
    for (k=0; k<tam; k++)
      for (i=0; i<tam; i++)
	matC[ind2d(i,j)] = matC[ind2d(i,j)] + matA[ind2d(i,k)]*matB[ind2d(k,j)];
}


int main(int argc, char *argv[]) {
  int i;
  int tam;
  float *matA, *matB, *matC;
  double t0, t1;
  
  if (argc != 2) {
    printf(" uso: <exec> <tamanho das matrizes> \n");
    exit(-1);
  }
  tam = atoi(argv[1]);

  // aloca e inicializa matrizes

  matA  = (float *) malloc (tam*tam*sizeof(float));
  matB  = (float *) malloc (tam*tam*sizeof(float));
  matC  = (float *) malloc (tam*tam*sizeof(float));
  for (i=0; i<tam*tam; i++) {
    matA[i]=1.0; matB[i]=1.0;
  }

  printf("tam=%d; tempos: \n", tam);

  for (i=0; i<tam*tam; i++) matC[i]=0.0;
  t0 = wall_time();
  MMijk(tam, matA, matB, matC);
  t1 = wall_time()-t0;
  printf("ijk=%f \n", t1);

  for (i=0; i<tam*tam; i++) matC[i]=0.0;
  t0 = wall_time();
  MMikj(tam, matA, matB, matC);
  t1 = wall_time()-t0;
  printf("ikj=%f \n", t1);

  for (i=0; i<tam*tam; i++) matC[i]=0.0;
  t0 = wall_time();
  MMkij(tam, matA, matB, matC);
  t1 = wall_time()-t0;
  printf("kij=%f \n", t1);

  for (i=0; i<tam*tam; i++) matC[i]=0.0;
  t0 = wall_time();
  MMkji(tam, matA, matB, matC);
  t1 = wall_time()-t0;
  printf("kji=%f \n", t1);

  for (i=0; i<tam*tam; i++) matC[i]=0.0;
  t0 = wall_time();
  MMjik(tam, matA, matB, matC);
  t1 = wall_time()-t0;
  printf("jik=%f \n", t1);

  for (i=0; i<tam*tam; i++) matC[i]=0.0;
  t0 = wall_time();
  MMjki(tam, matA, matB, matC);
  t1 = wall_time()-t0;
  printf("jki=%f \n", t1);

  free(matA);
  free(matB);
  free(matC);
  exit(0);    
}
