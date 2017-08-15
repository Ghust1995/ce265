#include <stdio.h>
#include <stdlib.h>
#include <omp.h> 

void InitTabul(int*, int*, int);
void DumpTabul(int*, int, int, int, char*);
void UmaVida(int*, int*, int);
int Correto(int*, int);


int main(int argc, char *argv[]) {
  int i;
  int tam=6;
  int* tabulIn;
  int* tabulOut;
  int numThreads, numProcs;
  char msg[9];
  
  // numero de threads e dump inicial

#ifdef _OPENMP
  numThreads=omp_get_max_threads();
  numProcs=omp_get_num_procs();
#else
  numThreads=1;
  numProcs=1;
#endif
  printf("Execucao com %d threads em %d processadores\n", numThreads, numProcs);

  // aloca e inicializa tabuleiros

  tabulIn  = (int *) malloc ((tam+2)*(tam+2)*sizeof(int));
  tabulOut = (int *) malloc ((tam+2)*(tam+2)*sizeof(int));
  InitTabul(tabulIn, tabulOut, tam);

  // dump tabuleiro inicial

  DumpTabul(tabulIn, tam, 1, tam, "Inicial");

  for (i=0; i<2*(tam-3); i++) {

    UmaVida(tabulIn, tabulOut, tam);
    sprintf(msg,"Iter %3.3d\0", 2*i+1);
    DumpTabul(tabulOut, tam, 1, tam, msg);

    UmaVida(tabulOut, tabulIn, tam);
    sprintf(msg,"Iter %3.3d\0", 2*i+2);
    DumpTabul(tabulIn, tam, 1, tam, msg);
  }

  if (Correto(tabulIn, tam)) 
    printf("**RESULTADO CORRETO**\n");
  else
    printf("**RESULTADO ERRADO**\n");

  free(tabulIn);
  free(tabulOut);

  exit(0);    
}
