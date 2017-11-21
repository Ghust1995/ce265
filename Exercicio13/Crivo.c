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

<<<<<<< HEAD
double wall_time(void);

=======
#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

#define MAX_SUBDIVISION 8000

double wall_time(void);

typedef struct ListaPrimos {
  long val;
  int indMult;
  struct ListaPrimos *next;
} ListaPrimos;

>>>>>>> 31ea1dc17e896b405b6aa128b2228e989d18d7c0

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
<<<<<<< HEAD
	printf("\n"); nPrint=0;
=======
        printf("\n"); nPrint=0;
>>>>>>> 31ea1dc17e896b405b6aa128b2228e989d18d7c0
      }
    }
  }
  if (nPrint != 0) printf("\n");
}

<<<<<<< HEAD
 
=======

>>>>>>> 31ea1dc17e896b405b6aa128b2228e989d18d7c0
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
<<<<<<< HEAD
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

  
=======
  long indInicial;
  long ind;
  long subdiv;
  long tamSubdiv = min(indMax, MAX_SUBDIVISION);

  ListaPrimos *listaPrimos = (ListaPrimos *) malloc(sizeof(ListaPrimos));
  ListaPrimos *fimListaPrimos;
  ListaPrimos *primoTeste;
  listaPrimos->next = NULL;

  primoTeste = listaPrimos;
  fimListaPrimos = listaPrimos;

  // Inicializar com 2
  primoTeste->val = 3;
  primoTeste->indMult = num2ind(3);

  for (subdiv = 0; subdiv <= indMax; subdiv += tamSubdiv) {
#ifdef _DEBUG
    printf("CALCULANDO PRIMOS DA SUBDIVISAO %ld \n", subdiv/tamSubdiv);
    fflush(stdout);
#endif
    // Limpar todos os primos já encontrados
    for(primoTeste = listaPrimos; primoTeste != NULL; primoTeste = primoTeste->next) {
      base = primoTeste->val;
      indInicial = primoTeste->indMult;

      // remove os multiplos da base a partir do indice inicial

#ifdef _DEBUG
      printf("LIMPANDO PRIMOS de %ld a %ld\n", max(indInicial, base*base), ind2num(subdiv + tamSubdiv));
#endif

      for (ind=max(indInicial, num2ind(base*base)); ind <= subdiv + tamSubdiv; ind += base) {
        primos[ind]=0;
        primoTeste->indMult = ind;
      }

#ifdef _DEBUG
      printf("Apos remover multiplos de %ld\n", base);
      prPrimos(primos,subdiv + tamSubdiv);
#endif

      if(primoTeste->next != NULL) continue;

#ifdef _DEBUG
      printf("PROCURANDO NOVOS PRIMOS de %ld a %ld\n", ind2num(primoTeste->indMult + 1), ind2num(subdiv + tamSubdiv));
#endif

      // adiciona novos primos na lista
      for (indBase=indInicial + 1; indBase<=subdiv + tamSubdiv; indBase++)
        if (primos[indBase]) {
#ifdef _DEBUG
          printf("Novo primo encontrado: %ld indice %ld\n", ind2num(indBase), indBase);
#endif
          long nova_base = ind2num(indBase);
          if(nova_base > sqrtMax) {
            break;
          }
          ListaPrimos *novoPrimo = (ListaPrimos *) malloc(sizeof(ListaPrimos));
          novoPrimo->val = nova_base;
          novoPrimo->indMult = indBase;
          fimListaPrimos->next = novoPrimo;
          fimListaPrimos = fimListaPrimos->next;
          fimListaPrimos->next = NULL;
          break;
        }
    }
#ifdef _DEBUG
    printf("FIM-----------------------------\n");
#endif
  }

  // Limpar memoria
  for(primoTeste = listaPrimos; primoTeste != NULL; primoTeste = listaPrimos) {
    listaPrimos = listaPrimos->next;
    //printf("%ld ", primoTeste->val);
    free(primoTeste);
  }
  printf("\n");
}


>>>>>>> 31ea1dc17e896b405b6aa128b2228e989d18d7c0
// Crivo de Eratostenes


int main(int argc, char *argv[]) {
  long numMax;      // maior numero a testar; arredondado para impar
  long sqrtMax;     // raiz quadrada do maior numero arredondado para impar
  long indMax;      // quantos indices para testar de 3 ao maior numero
  int *primos;      // marca se primo ou nao
  double tstart, tnow, tinit, tcomp, ttot;  // tempos de execucao

  tstart=wall_time();
<<<<<<< HEAD
  
=======

>>>>>>> 31ea1dc17e896b405b6aa128b2228e989d18d7c0
  // ultimo inteiro

  if (argc < 2) {
    printf("ERRO: faltou argumento (inteiro maximo) do executavel\n");
    exit(-1);
  }
  numMax=atol(argv[1]);

  // critica dado de entrada e dump inicial
<<<<<<< HEAD
  
=======

>>>>>>> 31ea1dc17e896b405b6aa128b2228e989d18d7c0
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
<<<<<<< HEAD
  
=======

>>>>>>> 31ea1dc17e896b405b6aa128b2228e989d18d7c0
  if (numMax%2==0) numMax--;
  indMax = num2ind(numMax);
  sqrtMax = (long) sqrt(numMax);
#ifdef _DEBUG
  printf("Ha %ld impares a pesquisar de 3 a %ld; para em %ld\n", indMax+1, numMax, sqrtMax);
#endif
<<<<<<< HEAD
  
  // aloca primos
  
=======

  // aloca primos

>>>>>>> 31ea1dc17e896b405b6aa128b2228e989d18d7c0
  primos = (int *) malloc((indMax+1)*sizeof(int));

  // inicializa primos

  initPrimos(primos, indMax);
#ifdef _DEBUG
  printf("Dados iniciais:\n");
  prPrimos(primos,indMax);
#endif 
  tnow=wall_time(); tinit=tnow-tstart;
<<<<<<< HEAD
 
  // Crivo de Eratosthenes
  
=======

  // Crivo de Eratosthenes

>>>>>>> 31ea1dc17e896b405b6aa128b2228e989d18d7c0
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
<<<<<<< HEAD
	 ttot, tinit, tcomp, ttot-tinit-tcomp);
=======
      ttot, tinit, tcomp, ttot-tinit-tcomp);
>>>>>>> 31ea1dc17e896b405b6aa128b2228e989d18d7c0
  exit(0);
}
