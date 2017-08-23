#include <stdio.h>
#include <stdlib.h>
#include <omp.h> 
#include <math.h> 

// ind2num: retorna o inteiro (3<=impar<=numMax)
//          representado pelo indice i em primos (0<=i<=indMax)
#define ind2num(i)  (2*(i)+3)
// num2ind: retorna o indice (0<=i<=indMax) em primos
//          que representa o numero (3<=impar<=numMax)
#define num2ind(i)  (((i)-3)/2)

double wall_time(void);


// prPrimos: imprime os numeros marcados como primos para debug


void prPrimos(int *primos, long indMax) {
#define maxPrint 10
  long i, nPrint;
  printf(" 2;"); nPrint=1;
  for (i=0; i<=indMax; i++) {
    if (primos[i]) {
      printf(" %ld;",ind2num(i));
      if (++nPrint >= maxPrint) {
        printf("\n"); nPrint=0;
      }
    }
  }
  if (nPrint != 0) printf("\n");
}


// quantosPrimos: retorna o numero de elementos marcados 
//                como primos


long quantosPrimos(int *primos, long indMax) {
  long i, cnt;
  cnt=1;
  for (i=0; i<=indMax; i++) {
    if (primos[i]) cnt++;
  }
  return(cnt);
}


// Crivo: encontra os primos ate ind2num(indMax)


void Crivo(int *primos, long indMax) {
  long sqrtMax;
  long indBase;
  long base;
  long i;

  // raiz quadrada do ultimo inteiro eh o maior fator primo

  sqrtMax = (long) sqrt((long) ind2num(indMax));

  // primeira base

  indBase=0;
  base=3;

  // para todas as bases

  do {

    // marca como nao primo os multiplos da base a partir do seu quadrado

    #pragma omp parallel for
    for (i=num2ind(base*base); i<=indMax; i+=base) {
      primos[i]=0;
    }

    // avanca a base para o proximo primo

    for (indBase=indBase+1; indBase<=indMax; indBase++)
      if (primos[indBase]) {
        base = ind2num(indBase);
        break;
      }

    // enquanto base nao superar o final de primos e
    //               nao superar a raiz quadrada do ultimo inteiro

  }
  while (indBase <= indMax && base <= sqrtMax);
}


// Crivo de Eratostenes


int main(int argc, char *argv[]) {
  long numMax;      // maior numero a testar; arredondado para impar
  long indMax;      // quantos indices para testar de 3 ao maior numero
  long i;           // indice de laco
  int *primos;      // marca se primo ou nao
  double tstart, tnow, tinit, tcomp, ttot;  // tempos de execucao
  int numThreads;   // numero de threads na execucao

  tstart=wall_time();

  // ultimo inteiro

  if (argc < 2) {
    printf("ERRO: faltou argumento inteiro maximo\n");
    exit(-1);
  }
  numMax=atol(argv[1]);

  // critica dado de entrada e dump inicial

  if (numMax < 2) {
    printf("Nao ha primos menores ou iguais a %ld\n", numMax);
    exit(0);
  }
  else if (numMax == 2) {
    printf("2 eh o unico primo menor ou igual a 2\n");
    exit(0);
  }

#ifdef _OPENMP
#pragma omp parallel
  if (omp_get_thread_num() == 0)
    numThreads = omp_get_num_threads();
#else
  numThreads=1;
#endif

  // impoe numMax impar; quantos indices

  if (numMax%2==0) numMax--;
  indMax = num2ind(numMax);

  // aloca e inicializa primos

  primos = (int *) malloc((indMax+1)*sizeof(int));
  for (i=0; i<=indMax; i++)
    primos[i]=1;

  // Crivo de Eratosthenes

  tnow=wall_time(); tinit=tnow-tstart;
  Crivo(primos, indMax);
  tcomp=wall_time()-tnow;

  // mede tempo, imprime quantos primos, retorna area e imprime tempos

  printf("Encontrou %ld primos de 2 a %ld\n", quantosPrimos(primos,indMax), numMax);
  free(primos);
  ttot = wall_time()-tstart;
  printf("Threads=%d, Tempos: total=%lf, init=%lf, comp=%lf, resto=%lf\n",
      numThreads, ttot, tinit, tcomp, ttot-tinit-tcomp);
  exit(0);
}
