#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_MESSAGE_SIZE 1000000

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Status status;
    int next, prev;
    int message_size = 1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3) {
        if (rank == 0) {
            printf("This program requires exactly 3 processes.\n");
        }
        MPI_Finalize();
        return 0;
    }

    next = (rank + 1) % 3;
    prev = (rank + 2) % 3;

    char* message = (char*)malloc(MAX_MESSAGE_SIZE);

    while (message_size <= MAX_MESSAGE_SIZE) {
        if (rank == 0) {
            printf("Process 0 sending message of size %d to Process 1\n", message_size);
            MPI_Send(message, message_size, MPI_CHAR, next, 0, MPI_COMM_WORLD);
            MPI_Recv(message, message_size, MPI_CHAR, prev, 0, MPI_COMM_WORLD, &status);
            printf("Process 0 received message of size %d from Process 2\n", message_size);
        } else {
            MPI_Recv(message, message_size, MPI_CHAR, prev, 0, MPI_COMM_WORLD, &status);
            printf("Process %d received message of size %d from Process %d\n", rank, message_size, prev);
            MPI_Send(message, message_size, MPI_CHAR, next, 0, MPI_COMM_WORLD);
            printf("Process %d sending message of size %d to Process %d\n", rank, message_size, next);
        }

        message_size *= 2;
    }

    free(message);
    MPI_Finalize();

    return 0;
}