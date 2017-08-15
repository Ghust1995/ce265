#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define ind2d(i,j) (i)*(tam+2)+j

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
  int k;
  int sqtam = tam*tam;

#pragma omp parallel for private(i, j, vizviv)
  for (k=0; k<sqtam; k++) {
    i=k%tam + 1;
    j=k/tam + 1;
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



// DumpTabul: Imprime trecho do tabuleiro entre
//            as posicoes (first,first) e (last,last)
//            X representa celula viva
//            . representa celula morta



void DumpTabul(int * tabul, int tam, int first, int last, char* msg){
  int i, ij;

  printf("%s; Dump posicoes [%d:%d, %d:%d] de tabuleiro %d x %d\n", \
      msg, first, last, first, last, tam, tam);
  for (i=first; i<=last; i++) printf("="); printf("=\n");
  for (i=ind2d(first,0); i<=ind2d(last,0); i+=ind2d(1,0)) {
    for (ij=i+first; ij<=i+last; ij++)
      printf("%c", tabul[ij]? 'X' : '.');
    printf("\n");
  }
  for (i=first; i<=last; i++) printf("="); printf("=\n");
}



// InitTabul: Inicializa dois tabuleiros:
//            tabulIn com um veleiro no canto superior esquerdo
//            tabulOut com celulas mortas



void InitTabul(int* tabulIn, int* tabulOut, int tam){
  int ij;

  for (ij=0; ij<(tam+2)*(tam+2); ij++) {
    tabulIn[ij] = 0;
    tabulOut[ij] = 0;
  }

  tabulIn[ind2d(1,2)] = 1; 
  tabulIn[ind2d(2,3)] = 1; 
  tabulIn[ind2d(3,1)] = 1; 
  tabulIn[ind2d(3,2)] = 1; 
  tabulIn[ind2d(3,3)] = 1; 
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
