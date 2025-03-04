#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void main() {
    srand(time(NULL));
    int arr[10];
    int i = 0;
    #pragma omp parallel shared(arr, i)
    {
        int curr = (rand() % 100) + 1;
        #pragma omp critical
        {
            if (i < 10) {
                arr[i] = curr;
                i++;
            }
            
        }
        #pragma omp barrier
        int max = 0;
        #pragma omp single
        {
            for (int i = 0; i < 10; i++) {
                if (max < arr[i]) {
                    max = arr[i];
                }
                printf("%d, ", arr[i]);
            }
            printf("\nMAX: %d\n", max);
        }
        
    }
}