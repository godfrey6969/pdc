#include <stdio.h>
#include <omp.h>

int main() {
    int shared_var = 0;  // Shared variable
    omp_lock_t lock;     // Declare a lock

    // Initialize the lock
    omp_init_lock(&lock);

    // Parallel region
    #pragma omp parallel num_threads(4)
    {
        for (int i = 0; i < 1000; i++) {
            // Acquire the lock before modifying the shared variable
            omp_set_lock(&lock);
            shared_var++;  // Critical section
            // Release the lock after modifying the shared variable
            omp_unset_lock(&lock);
        }
    }

    // Destroy the lock
    omp_destroy_lock(&lock);

    printf("Final value of shared_var: %d\n", shared_var);
    return 0;
}
