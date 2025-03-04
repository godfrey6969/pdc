#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
void main() {
    int arr[10];
    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        arr[i] = rand() % 100;
        printf("%d, ", arr[i]);
    }
    printf("\n");
    omp_set_num_threads(8);
    int res = 0;
    double start = omp_get_wtime();
    # pragma omp parallel for reduction(+:res)
    for (int i = 0; i < 10; i++) {
        res += arr[i];
    }
    double end = omp_get_wtime();
    printf("\nSum: %d\n", res);
    printf("Time: %fs\n", end - start);
}