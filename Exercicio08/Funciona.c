#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

void InitTabul(int *, int *, int, int, int);
void DumpTabul(int *, int, int, int, char*, int, int, int, int);
void UmaVida(int *, int *, int, int, int, int, int);
int Correto(int *, int, int, int, int, int);


int main(int argc, char *argv[]) {
  int i;
  int tam=6;
  int* tabulIn;
  int* tabulOut;
  int numProc, myId;
  char msg[9];
  
  // numero de processos, qual processo eu sou (substituir por MPI)

  if (myId == 0) printf("Execucao com %d processadores\n", numProc);

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myId); 

  if (numProc <= tam)
  {
    int tamLocal = tam/numProc;
    // O resto vai ser colocado no processo 0
    if (myId == 0) {
      tamLocal += tam % numProc;
    }

    // Determinar a linha inicial do processo
    int linha = myId * (tam/numProc);
    // Somar os restos
    if (myId != 0) {
      linha += tam % numProc;
    }


    // aloca e inicializa tabuleiros
    tabulIn  = (int *) malloc ((tamLocal+2)*(tam+2)*sizeof(int));
    tabulOut = (int *) malloc ((tamLocal+2)*(tam+2)*sizeof(int));
    InitTabul(tabulIn, tabulOut, tam, tamLocal, linha);


    // dump tabuleiro inicial
   DumpTabul(tabulIn, tam, 1, tam, "Inicial", tamLocal, linha, myId, numProc);
/*
    for (i=0; i<2*(tam-3); i++) {
      UmaVida(tabulIn, tabulOut, tam, tamLocal, linha, myId, numProc);
      sprintf(msg,"Iter %3.3d\0", 2*i+1);
      DumpTabul(tabulIn, tam, 1, tam, msg, tamLocal, linha, myId, numProc);

      UmaVida(tabulOut, tabulIn, tam, tamLocal, linha, myId, numProc);
      sprintf(msg,"Iter %3.3d\0", 2*i+2);
      DumpTabul(tabulIn, tam, 1, tam, msg, tamLocal, linha, myId, numProc);
     }
*/

    Correto(tabulIn, tam, myId, tamLocal, linha, numProc);

   free(tabulIn);
   free(tabulOut);

  }

 
  // if (Correto(tabulIn, tam)) 
    // printf("**RESULTADO CORRETO**\n");
  // else
    // printf("**RESULTADO ERRADO**\n");

  MPI_Finalize();
  exit(0);    
}
