#include <stdio.h>
#include <omp.h>

// Function to compute Fibonacci recursively with OpenMP tasks
int fib(int n) {
    int x, y;
    if (n < 2) {
        return n;
    } else {
        #pragma omp task shared(x)
        x = fib(n - 1);

        #pragma omp task shared(y)
        y = fib(n - 2);

        #pragma omp taskwait  // Ensure that both tasks are complete before continuing
        return x + y;
    }
}

int main() {
    int n = 10;  // Fibonacci number to compute
    int result;

    // Parallel region with a single thread creating the first task
    #pragma omp parallel
    {
        #pragma omp single  // Single thread starts the computation
        {
            result = fib(n);
        }
    }

    printf("Fibonacci(%d) = %d\n", n, result);
    return 0;
}
