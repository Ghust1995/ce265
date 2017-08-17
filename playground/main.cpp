#include <omp.h>
#include <stdio.h>
#define NUM_ELEM 10
int func(int i)
{
    return i*i;
}

int main() {
    int a[NUM_ELEM];
    int b[NUM_ELEM];

    #pragma omp parallel for schedule(static, 1)
    for(int i = 1; i < NUM_ELEM; i++) {
        b[i] = func(i);
        a[i] = b[i-1] - b [i];
    }
}
