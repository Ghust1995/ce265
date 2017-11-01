#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(int argc, char *argv[]) {

  // para desenvolvimento

//#define VA "bin/VectorATeste.bin"
//#define VI "bin/VectorIndTeste.bin" 
//#define RES -1544794685
  //const int m=32;
  //const int n=16;

  // para entregar

  #define VA "bin/VectorA.bin"
  #define VI "bin/VectorInd.bin" 
  #define RES 48820191429
    const int m=16777216;
    const int n=2048;

  // le vetor A do arquivo bin/VectorA.bin

  FILE *fp1=NULL;
  fp1 = fopen(VA, "r");
  if (fp1 == NULL) {
    printf("Erro ao abrir o arquivo %s\n",VA);
    exit(-1);
  }

  int *VetorA;
  VetorA=(int*)malloc(m*sizeof(int));
  size_t sizeA;
  sizeA=fread(VetorA,sizeof(int),m,fp1); 
  if (sizeA != (size_t) m) {
    printf("Erro ao ler o arquivo %s\n",VA);
    exit(-1);
  }
  fclose(fp1);

  // le vetor VetorInd do arquivo bin/VectorInd.bin

  FILE *fp2=NULL;
  fp2 = fopen(VI, "r");
  if (fp2 == NULL) {
    printf("Erro ao abrir o arquivo %s\n",VI);
    exit(-1);
  }
  int *VetorInd;
  VetorInd=(int*)malloc(n*sizeof(int));
  size_t sizeInd;
  sizeInd=fread(VetorInd,sizeof(int),n,fp2); 
  if (sizeInd != (size_t) n) {
    printf("Erro ao ler o arquivo %s\n",VI);
    exit(-1);
  }
  fclose(fp2);

  // calcula sum(VetorA(VetorInd))

  long sum=0;
  int i;
  for (i=0; i<n; i++) sum += VetorA[VetorInd[i]];

  // verifica correcao

  if (sum == RES)
    printf("**RESULTADO CERTO** soma=%ld\n",sum);
  else
    printf("**RESULTADO ERRADO** soma=%ld, deveria ser %ld\n",sum,RES);
  exit(0);
}
