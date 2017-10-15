#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#define ind2d(i,j) (i)*(tam+2)+j
#define ALIVE 1
#define DEAD 0

double wall_time(void);

// UmaVida: Executa uma iteracao do Jogo da Vida
//          em tabuleiros de tamanho tam. Produz o tabuleiro
//          de saida tabulOut a partir do tabuleiro de entrada
//          tabulIn. Os tabuleiros tem (tam, tam) celulas
//          internas vivas ou mortas. Tabuleiros sao orlados 
//          por celulas eternamente mortas.
//          Tabuleiros sao dimensionados tam+2 x tam+2.



void UmaVida(int* tabulIn, int* tabulOut, int tam) {
  int i, j;
  int vizviv;
  
  for (i=1; i<=tam; i++) {
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
  if(i >= linha && i <= linha + tamLocal){
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

  SetTabul(tabulIn, tam, tamLocal, linha, 1, 2, ALIVE);
  SetTabul(tabulIn, tam, tamLocal, linha, 2, 3, ALIVE);
  SetTabul(tabulIn, tam, tamLocal, linha, 3, 1, ALIVE);
  SetTabul(tabulIn, tam, tamLocal, linha, 3, 2, ALIVE);
  SetTabul(tabulIn, tam, tamLocal, linha, 3, 3, ALIVE);
}

// Correto: Verifica se a configuracao final do tabuleiro
//          estah correta ou nao (veleiro no canto inferior direito)



int Correto(int* tabul, int tam, int myId, int tamLocal, int linha){
  int ij, cnt;

  int linhaInicio = max(linha - 1, 0); // Ghost zone
  int linhaFinal = min(linha + tamLocal, tam); // Ghost zone

  int isCorreto = 0;

  int linhaTeste;
  // Testar correcao de uma linha
  for (linhaTeste = linhaInicio; linhaTeste <= linhaFinal, linhaTeste++)
  {
    cnt = 0;
    for (ij = ind2d(linhaTeste, 0); ij < (ind2d(linhaTeste, 0) + (tam + 2)); ij++) {
      cnt = cnt + tabul[ij];
    }

    switch(linhaTeste)
    {
      case tam:
        isCorreto = 
          cnt == 3 && 
          tabul[ind2d(linhaTeste - linha, tam)] &&
          tabul[ind2d(linhaTeste - linha, tam-1)] &&
          tabul[ind2d(linhaTeste - linha, tam-2)];
        break;
      case tam-1:
        isCorreto = 
          cnt == 1 && 
          tabul[ind2d(linhaTeste - linha, tam)];
        break;
      case tam-2:
        isCorreto = 
          cnt == 1 && 
          tabul[ind2d(linhaTeste - linha, tam-1)];
        break;
      default:
        isCorreto = cnt == 0;
        break;
    }
  }

  if(myId == 0) {
    int processo = 1;
    for(processo = 1; processo <= numProc; processo++) {
      MPI_Recv(
          outroCorreto,
          1,
          MPI_INT,
          processo, // Processo atual considerando resto no primeiro
          0,
          MPI_COMM_WORLD,
          MPI_STATUS_IGNORE);

      isCorreto = isCorreto && outroCorreto;
    }
    if (isCorreto) 
      printf("Resultado final está correto!")
    else
      printf("Resultado final está incorreto!")
  }
  else
  {
    MPI_Send(
        isCorreto,
        1,
        MPI_INT,
        0,
        0,
        MPI_COMM_WORLD);
  }
}
