#include <stdio.h>
#include <stdlib.h>
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
  int i;

  if(myId == 0) {
    printf("%s; Dump posicoes [%d:%d, %d:%d] de tabuleiro %d x %d\n", msg, first, last, first, last, tam, tam);
    for (i=first; i<=last; i++) printf("="); printf("=\n");
  }
  for (int l = first; l <= last; l++) {
    i=ind2d(l);
    // Determinar para linha atual qual a thread que vai mandar mensagem
    if (myId == 0) {
      // Arrumar
      if (l < linha + tamLocal) {
        // Caso dentro dos dados da thread 0
        for (int ij = i + first; ij < i + last; ij++) {
          printf("%c", tabul[ij]? 'X' : '.');
        }

        printf("\n");
      }
      else {
        // Caso recebendo de algum lugar
        int * linhaOutro;

        MPI_Recv(
            &linhaOutro, 
            (tam + 2),
            MPI_INT,
            (l - tamLocal) / (tam/numProc) + 1, // Processo atual considerando resto no primeiro
            0,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE); 

        for (int k = 0; k < last - first; k++) {
          printf("%c", linhaOutro[k]? 'X' : '.');
        }

        printf("\n");
      }
    }
    else { // Not main process
      // Arrumar
      int * linha = (int *) malloc (tam + 2) * sizeof(int);
      for (int k = 0; k < last - first; k++) {
        linha[k] = tabul[ind2d(l - linha, k + first)];
      }
      MPI_Send(
          &linha, 
          (tam + 2),
          MPI_INT, 
          0, 
          0, 
          MPI_COMM_WORLD); 
    }

  }
  if(myId == 0) {
    for (i=first; i<=last; i++) printf("="); printf("=\n");
  }
}


void SetTabul(int* tabul, int tamLocal, int linha, int i, int j, int val){
  if(i >= linha - 1 && i <= linha + tamLocal){
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

  SetTabul(tabulIn, tamLocal, linha, 1, 2, ALIVE);
  SetTabul(tabulIn, tamLocal, linha, 2, 3, ALIVE);
  SetTabul(tabulIn, tamLocal, linha, 3, 1, ALIVE);
  SetTabul(tabulIn, tamLocal, linha, 3, 2, ALIVE);
  SetTabul(tabulIn, tamLocal, linha, 3, 3, ALIVE);
}

// Correto: Verifica se a configuracao final do tabuleiro
//          estah correta ou nao (veleiro no canto inferior direito)



int Correto(int* tabul, int tam){
  int ij, cnt;

  cnt = 0;
  for (ij=0; ij<(tam+2)*(tam+2); ij++)
    cnt = cnt + tabul[ij];

  return (cnt == 5 &&
      tabul[ind2d(tam-2,tam-1)] &&
      tabul[ind2d(tam-1,tam  )] &&
      tabul[ind2d(tam  ,tam-2)] &&
      tabul[ind2d(tam  ,tam-1)] &&
      tabul[ind2d(tam  ,tam  )]);
}
