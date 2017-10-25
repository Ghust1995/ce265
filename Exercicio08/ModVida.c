#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#define ind2d(i,j) (i)*(tam+2)+j
#define ALIVE 1
#define DEAD 0
#define max(a, b) a > b ? a : b
#define min(a, b) a < b ? a : b

double wall_time(void);

// UmaVida: Executa uma iteracao do Jogo da Vida
//          em tabuleiros de tamanho tam. Produz o tabuleiro
//          de saida tabulOut a partir do tabuleiro de entrada
//          tabulIn. Os tabuleiros tem (tam, tam) celulas
//          internas vivas ou mortas. Tabuleiros sao orlados 
//          por celulas eternamente mortas.
//          Tabuleiros sao dimensionados tam+2 x tam+2.



void UmaVida(int* tabulIn, int* tabulOut, int tam, int tamLocal, int linha, int myId, int numProc) {
  int i, j;
  int vizviv;
  
  for (i=1; i<=tamLocal; i++) {
    for (j= 1; j<=tam; j++) {
      vizviv = 
	tabulIn[ind2d(i-1,j-1)] + 
	tabulIn[ind2d(i-1,j  )] +
	tabulIn[ind2d(i-1,j+1)] + 
	tabulIn[ind2d(i  ,j-1)] + 
	tabulIn[ind2d(i  ,j+1)] + 
	tabulIn[ind2d(i+1,j-1)] + 
	tabulIn[ind2d(i+1,j  )] + 
	tabulIn[ind2d(i+1,j+1)];
      if (tabulIn[ind2d(i,j)] && vizviv < 2) 
	tabulOut[ind2d(i,j)] = 0;
      else if (tabulIn[ind2d(i,j)] && vizviv > 3) 
	tabulOut[ind2d(i,j)] = 0;
      else if (!tabulIn[ind2d(i,j)] && vizviv == 3) 
	tabulOut[ind2d(i,j)] = 1;
      else
	tabulOut[ind2d(i,j)] = tabulIn[ind2d(i,j)];
    }
  }

  // atualizar ghost zones
}

// DumpTabul: Imprime trecho do tabuleiro entre
//            as posicoes (first,first) e (last,last)
//            X representa celula viva
//            . representa celula morta



void DumpTabul(int * tabul, int tam, int first, int last, char* msg, int tamLocal, int linha, int myId, int numProc) {
  int i, l, ij, k;

  if(myId == 0) {
    printf("%s; Dump posicoes [%d:%d, %d:%d] de tabuleiro %d x %d\n", msg, first, last, first, last, tam, tam);
    for (i=first; i<=last; i++) printf("="); printf("=\n");
  }
  for (l = first; l <= last; l++) {
    i=ind2d(l, 0);
    // Determinar para linha atual qual a thread que vai mandar mensagem
    if (myId == 0) {
      if (l < linha + tamLocal + 1) {
        // Caso dentro dos dados da thread 0
        for (ij = i + first; ij <= i + last; ij++) {
          printf("%c", tabul[ij]? 'X' : '.');
        }

        printf("\n");
      }
      else {
        // Caso recebendo de algum lugar
        int * linhaOutro = (int *) malloc ((tam + 2) * sizeof(int));

        int processo = (l - tamLocal - 1) / (tam/numProc) + 1;

        MPI_Recv(
            linhaOutro, 
            (tam + 2),
            MPI_INT,
            processo, // Processo atual considerando resto no primeiro
            0,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE); 

        for (k = 0; k <= last - first; k++) {
          printf("%c", linhaOutro[k]? 'X' : '.');
        }

        printf("\n");
      }
    }
    else { // Not main process
      if (l < linha + tamLocal && l >= linha) {
	      // Arrumar
	      int * linhaSend = (int *) malloc ((tam + 2) * sizeof(int));
	      for (k = 0; k <= tam+2; k++) {
		linhaSend[k] = 0;
	      }
	      for (k = 0; k <= last - first; k++) {
		linhaSend[k] = tabul[ind2d(l - linha + 1, k + first)];
	      }
		/*
	      for (k = 0; k <= last - first; k++) {
		printf("%d ", linhaSend[k]);
	      }
		printf("\n");
		*/
	      MPI_Send(
		  linhaSend, 
		  (tam + 2),
		  MPI_INT, 
		  0, 
		  0, 
		  MPI_COMM_WORLD); 
      }
    }

  }
  if(myId == 0) {
    for (i=first; i<=last; i++) printf("="); printf("=\n");
  }
}


void SetTabul(int* tabul, int tam, int tamLocal, int linha, int i, int j, int val){
  if(i >= linha - 1 && i <= linha + tamLocal + 1){
    tabul[ind2d(i - linha, j)] = val;
  }
}

// InitTabul: Inicializa dois tabuleiros:
//            tabulIn com um veleiro no canto superior esquerdo
//            tabulOut com celulas mortas

void InitTabul(int* tabulIn, int* tabulOut, int tam, int tamLocal, int linha){
  int ij;

  for (ij=0; ij<(tam+2)*(tamLocal+2); ij++) {
    tabulIn[ij] = 0;
    tabulOut[ij] = 0;
  }

  SetTabul(tabulIn, tam, tamLocal, linha, 1 + tam/2, 2 + tam/2, ALIVE);
  SetTabul(tabulIn, tam, tamLocal, linha, 2 + tam/2, 3 + tam/2, ALIVE);
  SetTabul(tabulIn, tam, tamLocal, linha, 3 + tam/2, 1 + tam/2, ALIVE);
  SetTabul(tabulIn, tam, tamLocal, linha, 3 + tam/2, 2 + tam/2, ALIVE);
  SetTabul(tabulIn, tam, tamLocal, linha, 3 + tam/2, 3 + tam/2, ALIVE);

/*
  SetTabul(tabulIn, tam, tamLocal, linha, 1, 2, ALIVE);
  SetTabul(tabulIn, tam, tamLocal, linha, 2, 3, ALIVE);
  SetTabul(tabulIn, tam, tamLocal, linha, 3, 1, ALIVE);
  SetTabul(tabulIn, tam, tamLocal, linha, 3, 2, ALIVE);
  SetTabul(tabulIn, tam, tamLocal, linha, 3, 3, ALIVE);
*/
}

// Correto: Verifica se a configuracao final do tabuleiro
//          estah correta ou nao (veleiro no canto inferior direito)



int Correto(int* tabul, int tam, int myId, int tamLocal, int linha, int numProc) {
  int ij, cnt;

  int linhaInicio = max(linha, 0); // Ghost zone
  int linhaFinal = min(linha + tamLocal + 1, tam + 1); // Ghost zone

  int isCorreto = 0;

  int linhaTeste;
  // Testar correcao de uma linha
  for (linhaTeste = linhaInicio; linhaTeste <= linhaFinal; linhaTeste++)
  {
    cnt = 0;
    for (ij = 0; ij < tam + 2; ij++) {
      cnt = cnt + tabul[ind2d(linhaTeste - linha, ij)];
    }

      if (linhaTeste == tam)
        isCorreto = 
          cnt == 3 && 
          tabul[ind2d(linhaTeste - linha, tam)] &&
          tabul[ind2d(linhaTeste - linha, tam-1)] &&
          tabul[ind2d(linhaTeste - linha, tam-2)];
      else if( linhaTeste == tam-1)
        isCorreto = 
          cnt == 1 && 
          tabul[ind2d(linhaTeste - linha, tam)];
      else if (linhaTeste == tam-2)
        isCorreto = 
          cnt == 1 && 
          tabul[ind2d(linhaTeste - linha, tam-1)];
      else
        isCorreto = cnt == 0;

      printf("processo %d testando linha %d, linha %s, count %d \n", myId, linhaTeste, isCorreto ? "CORRETA" : "INCORRETA", cnt);
    }
    int *corretos = (int*) malloc (numProc * sizeof(int));
      MPI_Gather(
          &isCorreto, 1, MPI_INT,
          corretos, 1, MPI_INT,
          0, MPI_COMM_WORLD);

  if (myId == 0) {
    int processo = 1;
    for(processo = 1; processo < numProc; processo++) {
          isCorreto = isCorreto && corretos[processo];
    }
    if (isCorreto) 
      printf("Resultado final está correto! \n");
    else
      printf("Resultado final está incorreto! \n");
  } 
}
