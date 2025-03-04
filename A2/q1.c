#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

void parallel(int arr1[], int arr2[]) {
    int res = 0;
    double start = omp_get_wtime();
    #pragma omp parallel for reduction(+:res)
    for (int i = 0; i < 1000; i++) {
        res += (arr1[i] * arr2[i]);
    }
    double end = omp_get_wtime();
    printf("Parallel Dot product: %d\n", res);
    printf("Parallel Elapsed time: %f\n", end - start);
}

void main() {
    srand(time(NULL));
    int arr1[1000];
    int arr2[1000];
    for (int i = 0; i < 1000; i++) {
        arr1[i] = (rand() % 100) + 1;
        arr2[i] = (rand() % 100) + 1;
    }
    parallel(arr1, arr2);
    int res = 0;
    double start = omp_get_wtime();
    #pragma omp single
    {
        for (int i = 0; i < 1000; i++) {
            res += (arr1[i] * arr2[i]);
        }
    }
    double end = omp_get_wtime();
    printf("Single Dot product: %d\n", res);
    printf("Single Elapsed time: %f\n", end - start);
}