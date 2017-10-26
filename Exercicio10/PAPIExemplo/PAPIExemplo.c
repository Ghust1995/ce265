#include <stdio.h>
#include <stdlib.h>
#include <papi.h>
double wall_time();

// reportError: imprime o codigo de erro do PAPI e aborta
//              a computacao


void reportError(int code, char *name){
  printf("**(ERR)** PAPI returned error %d at %s\n", PAPI_strerror(code), name);
  exit(-1);
}


// main: exemplo de uso do PAPI


int main(int argc, char *argv[]) {
#define NUM_EVENTS 4
  int EventCode[NUM_EVENTS] = {PAPI_TOT_CYC, PAPI_LD_INS, PAPI_L1_DCM, PAPI_L2_DCM};
  char EventName[NUM_EVENTS][PAPI_MAX_STR_LEN];
  long long EventCount[NUM_EVENTS];
  int retValue;
#define NBR_REP 1024
  int rep;
#define VET_SIZE 262144
  int i;
  float x[VET_SIZE];
  float a;
  double t0, t1;
  
  // inicializa PAPI e seus contadores
  
  if ((retValue = PAPI_start_counters(EventCode, NUM_EVENTS)) != PAPI_OK)
    reportError(retValue, "PAPI_start_counters");

  // impime quantos contadores disponiveis e quais eventos solicitados

  printf("This processor has %d counters\n", PAPI_num_counters());
  printf("Selected PAPI events:");
  for (i=0; i<NUM_EVENTS; i++) {
    if ((retValue=PAPI_event_code_to_name(EventCode[i], EventName[i])) != PAPI_OK)
      reportError(retValue, "PAPI_event_code_to_name");
    printf(" %s;", EventName[i]);
  }
  printf("\n");
  
  // inicializa vetor

  for (i=0; i<VET_SIZE; i++)
    x[i]=1.0;

  // inicio do trecho a medir; le e zera contadores
  
  if ((retValue = PAPI_read_counters(EventCount, NUM_EVENTS)) != PAPI_OK)
    reportError(retValue, "first PAPI_read_counters");
  
  t0 = wall_time();
  a = 1.0;
  for (rep=0; rep<NBR_REP; rep++){
    for (i=0; i<VET_SIZE; i++)
      x[i] += a*x[i];
    a=-a;
  }
  t1 = wall_time()-t0;
  printf("avoid dead code removal by printing some x %f\n", x[1]);
    
  // fim do trecho a medir; le e reporta contadores
  
  if ((retValue = PAPI_read_counters(EventCount, NUM_EVENTS)) != PAPI_OK)
    reportError(retValue, "second PAPI_read_counters");
  printf("Time=%lf; ",t1);
  for (i=0; i<NUM_EVENTS; i++) {
    printf(" %s=%lld;", EventName[i], EventCount[i]);
  }
  printf("\n");

  // para os contadores 

  if ((retValue = PAPI_stop_counters(EventCount, NUM_EVENTS)) != PAPI_OK)
    reportError(retValue, "PAPI_stop_counters");
  exit(0);    
}
