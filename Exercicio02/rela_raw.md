Paralelização do Jogo da Vida com OpenMP
========================================

Modificações no código
----------------------

A principal alteração feita foi a transformação de dois loops for aninhados em um único loop:

```c
// Loops aninhados
for(int i = 1; i <= tam; i++) {
    for(int j = 1; j <= tam; j++) {
        // ... codigo usando i e j
    }
}

// Loop unico
for (k=0; k<sqtam; k++) {
    int i=k%tam + 1;
    int j=k/tam + 1;
    // ... mesmo codigo usando i e j
}
``` 

Como no caso do jogo da vida o código executado dentro do loop não depende de resultados anteriores do mesmo, a união torna possível paralelizar o código em até tam*tam (sqtam) threads, ao contrário do original que só poderia ser divido em tam threads.

Após a união dos loops bastou garantir que nenhuma váriavel pudesse causar conflito entre as diversas threads, especificamente as variaveis i, j e vizviv foram declaradas privadas à thread, garantindo assim, que o loop seja verdadeiramente paralelizável e sem conflitos.

Procedimento UmaVida Paralelizado
---------------------------------

```c
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
```

Arquivos de Saída Funciona.c
----------------------------

Tabelas de Tempo de Execução e Speed-Ups
----------------------------------------

- Tamanho 8

| Threads | Tempo (s) | Speed-Up |
| ------- | --------- | -------- |
| 1       | time8-1   | -        |
| 2       | time8-2   | sup8-2   |
| 4       | time8-4   | sup8-4   |
| 8       | time8-8   | sup8-8   |
| 16      | time8-16  | sup8-16  |

- Tamanho 16

| Threads | Tempo (s) | Speed-Up |
| ------- | --------- | -------- |
| 1       | time16-1  | -        |
| 2       | time16-2  | sup16-2  |
| 4       | time16-4  | sup16-4  |
| 8       | time16-8  | sup16-8  |
| 16      | time16-16 | sup16-16 |

- Tamanho 32

| Threads | Tempo (s) | Speed-Up |
| ------- | --------- | -------- |
| 1       | time32-1  | -        |
| 2       | time32-2  | sup32-2  |
| 4       | time32-4  | sup32-4  |
| 8       | time32-8  | sup32-8  |
| 16      | time32-16 | sup32-16 |

- Tamanho 64

| Threads | Tempo (s) | Speed-Up |
| ------- | --------- | -------- |
| 1       | time64-1  | -        |
| 2       | time64-2  | sup64-2  |
| 4       | time64-4  | sup64-4  |
| 8       | time64-8  | sup64-8  |
| 16      | time64-16 | sup64-16 |

- Tamanho 128

| Threads | Tempo (s) | Speed-Up |
| ------- | --------- | -------- |
| 1       | time128-1 | -        |
| 2       | time128-2 | sup128-2 |
| 4       | time128-4 | sup128-4 |
| 8       | time128-8 | sup128-8 |
| 16      | time128-16| sup128-16|

- Tamanho 256

| Threads | Tempo (s) | Speed-Up |
| ------- | --------- | -------- |
| 1       | time256-1 | -        |
| 2       | time256-2 | sup256-2 |
| 4       | time256-4 | sup256-4 |
| 8       | time256-8 | sup256-8 |
| 16      | time256-16| sup256-16|

- Tamanho 512

| Threads | Tempo (s) | Speed-Up |
| ------- | --------- | -------- |
| 1       | time512-1 | -        |
| 2       | time512-2 | sup512-2 |
| 4       | time512-4 | sup512-4 |
| 8       | time512-8 | sup512-8 |
| 16      | time512-16| sup512-16|


![speedup-chart]

Análise e Comentários
---------------------

Do gráfico que o speedup não é uma linha reta, ou seja, existe algum custo envolvido na paralelização, relacionado provavelmente à escrita e leitura em memória compartilhada entre as CPU's (respectivamente às variáveis tabulIn e tabulOut).

Além disso, do exercício pode-se entender a simplicidade de se tornar uma parte do código paralelizada a partir do uso do OpenMP, ao mesmo tempo que existe uma complexidade envolvida em garantir que o código seja paralelizável. Planejar o código para que variaveis 'globais' não entrem em conflito em multiplas threads foi o maior desafio desse exercício.:wa


[speedup-chart]: ./speedup.png