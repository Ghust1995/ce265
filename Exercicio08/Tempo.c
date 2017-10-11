#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

double wall_time(void);
void InitTabul(int*, int*, int);
void DumpTabul(int*, int, int, int, char*);
void UmaVida(int*, int*, int);
int Correto(int*, int);


int main(int argc, char *argv[]) {
#define POW 11
  int i;
  int tam;
  int* tabulIn;
  int* tabulOut;
  int numProc, myId;
  char msg[9];
  double t0, t1, t2, t3;
  
  // numero de processos, qual processo eu sou (substituir por MPI)

  numProc=1; 
  myId=0;
  if (myId == 0) printf("Execucao com %d processadores\n", numProc);

  // tamanho do tabuleiro

  tam = 1 << POW;

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
    printf("**RESULTADO CORRETO**\n");
  else
    printf("**RESULTADO ERRADO**\n");
    
  t3 = wall_time();
  printf("tam=%d; tempos: init=%f, comp=%f, fim=%f, tot=%f \n", 
	 tam, t1-t0, t2-t1, t3-t2, t3-t0);
  free(tabulIn);
  free(tabulOut);
  exit(0);    
}
