#include <stdio.h>
#include <stdlib.h>

// interfaces

double wall_time(void);
void UmaVida(int*, int*, int);
__global__ void UmaVidaGpu(int*, int*, int);
void InitTabul(int*, int*, int);
void DumpTabul(int*, int, int, int, char*);
int Correto(int*, int);



// Trafega um veleiro de tamanho tam ao longo do tabuleiro
// Tamanho tam eh produto dos argumentos de entrada tamBlk e nBlk
// Tamanho minimo eh MinTam




int main(int argc, char *argv[]) {
#define MinTam 4
  int i;
  int tam, tamBlk, nBlk;
  int* tabulIn;
  int* tabulOut;
  size_t size;
  double t0, t1, t2, t3;
  char msg[16];
  
  // obtem tamanho do tabuleiro

  if (argc != 3) {
    printf(" uso: <exec> <celulas por bloco> <quantos blocos>\n");
    exit(-1);
  }
  tamBlk = atoi(argv[1]);
  nBlk = atoi(argv[2]);
  tam = nBlk*tamBlk;
  size = (tam+2)*(tam+2)*sizeof(int);

  // tamanho minimo
  if (tam < MinTam) {
    printf("**ERRO** tamanho %d menor que o minimo %d\n", tam, MinTam);
    exit(-1);
  }

  // CPU
  // aloca e inicializa tabuleiros

  t0 = wall_time();
  tabulIn  = (int *) malloc (size);
  tabulOut = (int *) malloc (size);
  InitTabul(tabulIn, tabulOut, tam);

  // dump tabuleiro inicial

  sprintf(msg,"Inicial CPU");
  DumpTabul(tabulIn, tam, 1, 4, msg);

  // avanca geracoes

  t1 = wall_time();
  for (i=0; i<2*(tam-3); i++) {
    UmaVida (tabulIn, tabulOut, tam);
    UmaVida (tabulOut, tabulIn, tam);
  }
  t2 = wall_time();

  // dump tabuleiro final

  sprintf(msg,"Final CPU");
  DumpTabul(tabulIn, tam, tam-3, tam, msg);

  // Correcao na CPU

  if (Correto(tabulIn, tam)) 
    printf("**RESULTADO CORRETO NA CPU**\n");
  else
    printf("**RESULTADO ERRADO NA CPU**\n");

  t3 = wall_time();

  // Tempos na CPU

  printf("tam=%d; tempos na CPU: init=%f, comp=%f, fim=%f, tot=%f \n", 
  //js printf("module.exports = {tam:%d, \n")
  //js printf("cpu: {init:%f, comp:%f, fim:%f, tot:%f }, \n", 
	 tam, t1-t0, t2-t1, t3-t2, t3-t0);

  // GPU
  // aloca e inicializa tabuleiros
  int* tabulIn_d;
  int* tabulOut_d;

  t0 = wall_time();
  tabulIn  = (int *) malloc (size);
  cudaMemcpy(tabulIn_d, tabulIn, size*sizeof(int), cudaMemcpyHostToDevice);
  tabulOut = (int *) malloc (size);
  cudaMemcpy(tabulOut_d, tabulOut, size*sizeof(int), cudaMemcpyHostToDevice);
  dim3 dB (tamBlk, tamBlk);
  dim3 dG (nBlk, nBlk);
  InitTabul(tabulIn, tabulOut, tam);

  // dump tabuleiro inicial

  sprintf(msg,"Inicial GPU");
  DumpTabul(tabulIn, tam, 1, 4, msg);

  // avanca geracoes

  t1 = wall_time();
  for (i=0; i<2*(tam-3); i++) {
    UmaVidaGpu <<< dG, dB >>> (tabulIn, tabulOut, tam);
    UmaVidaGpu <<< dG, dB >>> (tabulOut, tabulIn, tam);
  }
  t2 = wall_time();

  // Pega as variaveis de volta da gpu
  cudaMemcpy(tabulIn, tabulIn_d, size*sizeof(int), cudaMemcpyDeviceToHost);
  cudaMemcpy(tabulOut, tabulOut_d, size*sizeof(int), cudaMemcpyDeviceToHost);

  // dump tabuleiro final

  sprintf(msg,"Final GPU");
  DumpTabul(tabulIn, tam, tam-3, tam, msg);

  // Correcao na GPU

  if (Correto(tabulIn, tam)) 
    printf("**RESULTADO CORRETO NA GPU**\n");
  else
    printf("**RESULTADO ERRADO NA GPU**\n");

  t3 = wall_time();

  // Tempos na GPU

  printf("tam=%d; tempos na GPU: init=%f, comp=%f, fim=%f, tot=%f \n", 
  //js printf("gpu: {init:%f, comp:%f, fim:%f, tot:%f },\n", 
	 tam, t1-t0, t2-t1, t3-t2, t3-t0);

  //js printf("}\n")

  free(tabulIn);
  free(tabulOut);

  exit(0);    
}
