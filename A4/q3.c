#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 100



int privateVar = 0;
#pragma omp threadprivate(privateVar)

int main() {
    int mat[SIZE][SIZE], vec[SIZE], result[SIZE];
    int sum = 0, max = -1;
    srand(42);
    for (int i = 0; i < SIZE; i++) {
        vec[i] = rand() % 100;
        for (int j = 0; j < SIZE; j++) {
            mat[i][j] = rand() % 100;
        }
    }
    #pragma omp parallel shared(mat, vec, result, sum, max) 
    {
        #pragma omp single
        {
            printf("Starting parallel execution...\n");
        }
        #pragma omp for reduction(+:result[:SIZE])
        for (int i = 0; i < SIZE; i++) {
            result[i] = 0;
            for (int j = 0; j < SIZE; j++) {
                result[i] += mat[i][j] * vec[j];
            }
        }

        #pragma omp sections
        {
            #pragma omp section
            {
                int localSum = 0;
                for (int i = 0; i < SIZE; i++) {
                    localSum += result[i];
                }
                #pragma omp atomic
                sum += localSum;
            }

            #pragma omp section
            {
                int localMax = result[0];
                for (int i = 1; i < SIZE; i++) {
                    if (result[i] > localMax) {
                        localMax = result[i];
                    }
                }
                if (localMax > max) {
                	max = localMax;
                }
                
                
            }
        }
    }
    printf("Sum of result: %d\n", sum);
    printf("Max value in result: %d\n", max);

    return 0;
}

