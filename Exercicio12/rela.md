Utilização de RMA com MPI 2.0 para acesso de memória remota
===========================================================

Descrição da solução paralela
-----------------------------

Para essa solução paralela foram usados os conceitos de window do MPI e a função MPI_Get, de forma a fazer a comunicação unilateral entre os processos, podendo cada um deles acessar a memória dos outros conforme a necessidade.

Cada vez que se calcula um novo ind[i] checa-se a qual processo esse valor pertence, caso seja local, basta acessar o array A no displacement correto, caso seja remoto, basta usar a função MPI_Get para acessar o processo correto no displacement calculado.

Após calcular cada soma local, utiliza-se a função MPI_Reduce para colocar a soma total no processo 0.

Parte modificada de SomaPar.c
-----------------------------
```c
```

Arquivos de saída 
----------
- SomaPar_1MPI.out
```

```

- SomaPar_2MPI.out
```

```

- SomaPar_4MPI.out
```

```

- SomaPar_8MPI.out
```

```

- SomaPar_16MPI.out
```

```

- SomaPar_32MPI.out
```

```

- SomaPar_64MPI.out
```

```

Análise
-------

Observando os resultados obtidos, percebe-se a diferença que o bom planejamento da arquitetura de memória de um programa pode causar, com um aumento no tempo de mais de três vezes do programa mais eficiente até o menos eficiente.

Percebe-se tambem que existe correlação entre os acertos na cache L1 e na cache L2, aumentando ainda mais a eficiencia dos programas que utilizam a cache de forma planejada.


Programa SomaPar.c completo
---------------------------
```c
```
