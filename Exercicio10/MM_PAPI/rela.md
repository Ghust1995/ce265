Analise da eficiência de cache na Multiplicação de Matrizes
===========================================================

Programa MM.c alterado
----------------------
```c

```

Resultados
----------

| Multiplicação | Tempo | L1 Cache Miss % | L2 Cache Miss % |
| ------------- | ----- | --------------- | --------------- |
|      ijk      |
|      ikj      |
|      kij      |
|      kji      |
|      jik      |
|      jki      |
![Comparação entre os tempos de execução do loop principal na CPU e na GPU para a primeira tarefa][cpu-gpu-comparison1-loop]

Análise
-------

Tempos de execução total da primeira tarefa
-------------------------------------------

| nBlk | Tempo CPU (s) | Tempo GPU (s) |
| ---- | ------------- | ------------- |
| 4    | cpuT1-4       | gpuT1-4       |
| 8    | cpuT1-8       | gpuT1-8       |
| 16   | cpuT1-16      | gpuT1-16      |
| 32   | cpuT1-32      | gpuT1-32      |
| 64   | cpuT1-64      | gpuT1-64      |

![Comparação entre os tempos de execução total na CPU e na GPU para a primeira tarefa][cpu-gpu-comparison1-total]

Análise
-------

Tempos de execução do laço principal para segunda tarefa
---------------------------------------------------------

| nBlk | Tempo CPU (s) | Tempo GPU (s) |
| ---- | ------------- | ------------- |
| 4    | cpuL2-4       | gpuL2-4       |
| 8    | cpuL2-8       | gpuL2-8       |
| 16   | cpuL2-16      | gpuL2-16      |
| 32   | cpuL2-32      | gpuL2-32      |
| 64   | cpuL2-64      | gpuL2-64      |

![Comparação entre os tempos de execução do loop principal na CPU e na GPU para a segunda tarefa][cpu-gpu-comparison2-loop]

Análise
-------

Tempos de execução total da segunda tarefa
-------------------------------------------

| nBlk | Tempo CPU (s) | Tempo GPU (s) |
| ---- | ------------- | ------------- |
| 4    | cpuT2-4       | gpuT2-4       |
| 8    | cpuT2-8       | gpuT2-8       |
| 16   | cpuT2-16      | gpuT2-16      |
| 32   | cpuT2-32      | gpuT2-32      |
| 64   | cpuT2-64      | gpuT2-64      |

![Comparação entre os tempos de execução total na CPU e na GPU para a segunda tarefa][cpu-gpu-comparison2-total]

Análise
-------

Conclusões
----------

[cpu-gpu-comparison1-loop]: ./cpu-gpu-comparison1-loop.png
[cpu-gpu-comparison1-total]: ./cpu-gpu-comparison1-total.png
[cpu-gpu-comparison2-loop]: ./cpu-gpu-comparison2-loop.png
[