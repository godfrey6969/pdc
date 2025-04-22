#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define NUMBER_OF_TESTS 10  // for more reliable average results

int main(int argc, char* argv[]) {
    double *buf;
    int rank, numprocs;
    int i, k, x;
    double t1, t2, tloop;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    if (numprocs != 2) {
        printf("The number of processes must be two!\n");
        return(0);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("# size [B]\ttime [sec]\trate [MB/sec]\n");
    }

    for (x = 1; x <= 10000000; x *= 2) {  // message length doubles
        tloop = 0.0;
        if (x > 100000) i = 1;  // just a single loop for long messages
        else i = NUMBER_OF_TESTS;

        buf = (double *)malloc(x * sizeof(double));
        if (!buf) {
            printf("Could not allocate message buffer of size [%d]\n", x);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        for (k = 0; k < NUMBER_OF_TESTS; k++) {
            if (rank == 0) {
                t1 = MPI_Wtime();
                for (j = 0; j < i; j++) {  // send message xloop times
                    MPI_Send(buf, x, MPI_DOUBLE, 1, k, MPI_COMM_WORLD);
                }
                t2 = MPI_Wtime() - t1;
                tloop += t2;
            } else if (rank == 1) {
                for (j = 0; j < i; j++) {  // receive message xloop times
                    MPI_Recv(buf, x, MPI_DOUBLE, 0, k, MPI_COMM_WORLD, &status);
                }
            }
        }

        if (rank == 0) {  // calculate bandwidth
            double bandwidth;
            bandwidth = x * i * sizeof(double) * 1.0e-6 / (tloop / NUMBER_OF_TESTS);  // in MB/sec
            printf("%10.0f\t%10.6f\t%10.2f\n", x * sizeof(double), tloop / NUMBER_OF_TESTS, bandwidth);
        }

        free(buf);
    }

    MPI_Finalize();
    return 0;
}
