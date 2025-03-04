#include <stdio.h>
#include <omp.h>

void main() {
    omp_set_num_threads(10);
    int arr[10];
    for (int i = 0; i < 10; i++) {
        arr[i] = 0;
    }
    #pragma omp parallel
    {
        int curr = omp_get_thread_num();
        for (int i = 0; i < 10; i++) {
            arr[curr] += curr;
        }
    }
    for (int i = 0; i < 10; i++) {
        printf("Thread: %d, Sum: %d\n", i, arr[i]);
    }
}