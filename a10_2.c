#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int n = 2;
    int *data = NULL;
    int *recv_buf = (int *)malloc(n * sizeof(int));
    int *gathered = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0)
            printf("This program must be run with 4 processes.\n");
        MPI_Finalize();
        return 1;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
        printf("Broadcasting n = %d\n", n);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        data = (int *)malloc(4 * n * sizeof(int));
        for (int i = 0; i < 4 * n; ++i)
            data[i] = i + 1;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatter(data, n, MPI_INT, recv_buf, n, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Process %d received: ", rank);
    for (int i = 0; i < n; ++i) {
        recv_buf[i] *= 2;
        printf("%d ", recv_buf[i]);
    }
    printf("\n");

    if (rank == 0)
        gathered = (int *)malloc(4 * n * sizeof(int));

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(recv_buf, n, MPI_INT, gathered, n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Final gathered array: ");
        for (int i = 0; i < 4 * n; ++i)
            printf("%d ", gathered[i]);
        printf("\n");
        free(data);
        free(gathered);
    }

    free(recv_buf);
    MPI_Finalize();
    return 0;
}