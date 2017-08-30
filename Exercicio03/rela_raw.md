Paralelização do Crivo de Erastótenes com OpenMP
================================================

Modificações no código
----------------------

As modificações feitas no código foram muito triviais, bastando utilizar o OpenMP para paralelizar o loop for que marca os primos no crivo. Como o loop que calcula o proximo valor primo não possui um número determinado de iterações e tem objetivo inerentemente sequencial (encontrar o primeiro valor que é primo), ele não foi paralelizado.

Procedimento Crivo Paralelizado
---------------------------------

```c
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
```

Tempos de execução e Speed-ups sem otimizações
----------------------------------------------

| Threads | Tempo (s) | Speed-Up |
| ------- | --------- | -------- |
| 1       | time1-1   |  -       |
| 2       | time1-2   | sup1-2   |
| 4       | time1-4   | sup1-4   |
| 8       | time1-8   | sup1-8   |
| 12      | time1-12  | sup1-12  |
| 16      | time1-16  | sup1-16  |
| 20      | time1-20  | sup1-20  |
| 24      | time1-24  | sup1-24  |

![speedup-chart-no-opt]

Análise
-------

Tempos de execução e Speed-ups com otimizações
----------------------------------------------

| Threads | Tempo (s) | Speed-Up |
| ------- | --------- | -------- |
| 1       | time2-1   |  -       |
| 2       | time2-2   | sup2-2   |
| 4       | time2-4   | sup2-4   |
| 8       | time2-8   | sup2-8   |
| 12      | time2-12  | sup2-12  |
| 16      | time2-16  | sup2-16  |
| 20      | time2-20  | sup2-20  |
| 24      | time2-24  | sup2-24  |

![speedup-chart-o3]

Análise
-------

Conclusões
----------

[speedup-chart-no-opt]: ./speedup-no-opt.png
[speedup-chart-o3]: ./speedup-o3.png