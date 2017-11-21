#include <stdio.h>
#include <stdlib.h>
#include <omp.h> 
#include <math.h> 
//#define _DEBUG
#undef _DEBUG
// ind2num: retorna o inteiro (3<=impar<=numMax)
//          representado pelo indice i em primos (0<=i<=indMax)
#define ind2num(i)  (2*(i)+3)
// num2ind: retorna o indice (0<=i<=indMax) em primos
//          que representa o numero (3<=impar<=numMax)
#define num2ind(i)  (((i)-3)/2)

double wall_time(void);


// initPrimos: inicializa primos admitindo que todos
//             sao primos ate prova ao contrario


void initPrimos(int *primos, long indMax) {
  long i;
  for (i=0; i<=indMax; i++)
    primos[i]=1;
}


// prPrimos: imprime os numeros marcados como primos


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


// Crivo: encontra os primos ate indMax


void Crivo(int *primos, long indMax, long sqrtMax) {
  long indBase;
  long base;
  long indQuadrado;
  long i;
  
  indBase=0;
  base=3;
  do {

    // remove os multiplos da base a partir do quadrado da base
    
    indQuadrado=num2ind(base*base);
    for (i=indQuadrado; i<=indMax; i+=base)
      primos[i]=0;

#ifdef _DEBUG
    printf("Apos remover multiplos de %ld\n", base);
    prPrimos(primos,indMax);
#endif

    // avanca a base para o proximo primo

    for (indBase=indBase+1; indBase<=indMax; indBase++)
      if (primos[indBase]) {
	base = ind2num(indBase);
	break;
      }

#ifdef _DEBUG
    printf("proximo primo=%ld\n", base);
#endif
  }
  while (indBase <= indMax && base <= sqrtMax);
}

  
// Crivo de Eratostenes


int main(int argc, char *argv[]) {
  long numMax;      // maior numero a testar; arredondado para impar
  long sqrtMax;     // raiz quadrada do maior numero arredondado para impar
  long indMax;      // quantos indices para testar de 3 ao maior numero
  int *primos;      // marca se primo ou nao
  double tstart, tnow, tinit, tcomp, ttot;  // tempos de execucao

  tstart=wall_time();
  
  // ultimo inteiro

  if (argc < 2) {
    printf("ERRO: faltou argumento (inteiro maximo) do executavel\n");
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
  else
    printf("Calcular primos menores que %ld\n", numMax);

  // impoe numMax impar; quantos indices; raiz quadrada de numMax
  
  if (numMax%2==0) numMax--;
  indMax = num2ind(numMax);
  sqrtMax = (long) sqrt(numMax);
#ifdef _DEBUG
  printf("Ha %ld impares a pesquisar de 3 a %ld; para em %ld\n", indMax+1, numMax, sqrtMax);
#endif
  
  // aloca primos
  
  primos = (int *) malloc((indMax+1)*sizeof(int));

  // inicializa primos

  initPrimos(primos, indMax);
#ifdef _DEBUG
  printf("Dados iniciais:\n");
  prPrimos(primos,indMax);
#endif 
  tnow=wall_time(); tinit=tnow-tstart;
 
  // Crivo de Eratosthenes
  
  Crivo(primos, indMax, sqrtMax);

  // mede tempo, imprime quantos primos, retorna area e imprime tempos

  tcomp=wall_time()-tnow;
  printf("Encontrou %ld primos\n", quantosPrimos(primos,indMax));
#ifdef _DEBUG
  prPrimos(primos,indMax);
#endif
  free(primos);
  ttot = wall_time()-tstart;
  printf("Tempos: total=%f, init=%f, comp=%f, resto=%f\n",
	 ttot, tinit, tcomp, ttot-tinit-tcomp);
  exit(0);
}
