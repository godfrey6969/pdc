#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NUMBER_OF_TESTS 100

int main(int argc, char *argv[]) {
    int rank, size;
    int message_sizes[] = {1, 10, 100, 1000};
    int num_sizes = sizeof(message_sizes) / sizeof(int);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    for (int s = 0; s < num_sizes; ++s) {
        int count = message_sizes[s];
        double *message = (double *)malloc(count * sizeof(double));
        double total_time = 0.0;

        for (int test = 0; test < NUMBER_OF_TESTS; ++test) {
            MPI_Barrier(MPI_COMM_WORLD);
            double start_time = MPI_Wtime();
            if (rank == 0) {
                MPI_Send(message, count, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(message, count, MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                MPI_Recv(message, count, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                int next = (rank + 1) % size;
                MPI_Send(message, count, MPI_DOUBLE, next, 0, MPI_COMM_WORLD);
            }
            double end_time = MPI_Wtime();
            if (rank == 0)
                total_time += (end_time - start_time);
        }
        if (rank == 0) {
            double avg_latency = (total_time / NUMBER_OF_TESTS) * 1000;
            printf("%10d\t\t%.6f\n", count, avg_latency);
        }

        free(message);
    }

    MPI_Finalize();
    return 0;
}