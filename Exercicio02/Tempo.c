#include <stdio.h>
#include <stdlib.h>
#include <omp.h> 

double wall_time(void);
void InitTabul(int*, int*, int);
void DumpTabul(int*, int, int, int, char*);
void UmaVida(int*, int*, int);
int Correto(int*, int);


int main() {
#define POWMIN 3
#define POWMAX 9
  int pow;
  int i;
  int tam;
  int* tabulIn;
  int* tabulOut;
  int numThreads, numProcs;
  char msg[9];
  double t0, t1, t2, t3;
  
  // numero de threads e processadores

#ifdef _OPENMP
  numThreads=omp_get_max_threads();
  numProcs=omp_get_num_procs();
#else
  numThreads=1;
  numProcs=1;
#endif

  // para todos os tamanhos do tabuleiro

  for (pow=POWMIN; pow<=POWMAX; pow++){

    tam = 1 << pow;
    // aloca e inicializa tabuleiros

    t0 = wall_time();
    tabulIn  = (int *) malloc ((tam+2)*(tam+2)*sizeof(int));
    tabulOut = (int *) malloc ((tam+2)*(tam+2)*sizeof(int));
    InitTabul(tabulIn, tabulOut, tam);
    
    t1 = wall_time();
    for (i=0; i<2*(tam-3); i++) {
      
      UmaVida(tabulIn, tabulOut, tam);
      
      UmaVida(tabulOut, tabulIn, tam);
    }
    t2 = wall_time();
    
    if (Correto(tabulIn, tam)) 
      printf("**RESULTADO CORRETO; threads=%d; procs=%d; ", numThreads, numProcs);
    else {
      printf("**RESULTADO ERRADO**\n");
      exit(-1);
    }
    
    t3 = wall_time();
    printf("tam=%d; tempos: init=%f, comp=%f, fim=%f, tot=%f \n", 
	   tam, t1-t0, t2-t1, t3-t2, t3-t0);
    free(tabulIn);
    free(tabulOut);
  }
  exit(0);    
}
