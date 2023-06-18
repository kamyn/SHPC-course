#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main()
{
    int n = 1e3;
    int i, j, k;
    int **A, **B, **C;

    MPI_Init(NULL, NULL);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int rowsPerProc = n / size;
    int rowsRemain = n % size;
    int startRow = rank * rowsPerProc;
    int endRow = startRow + rowsPerProc;

    if (rank == size - 1)
        endRow += rowsRemain;

    A = (int **)malloc(n * sizeof(int *));
    B = (int **)malloc(n * sizeof(int *));
    C = (int **)malloc(n * sizeof(int *));

    for (i = 0; i < n; ++i) {
        A[i] = (int *)malloc(n * sizeof(int));
        B[i] = (int *)malloc(n * sizeof(int));
        C[i] = (int *)malloc(n * sizeof(int));
    }

    srand(time(NULL));
// generate random matrix with elements between -100 and 100
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            A[i][j] = rand() % 201 - 100;
            B[i][j] = rand() % 201 - 100;
            C[i][j] = 0;
        }
    }

    double start = MPI_Wtime();

    for (i = startRow; i < endRow; ++i) {
        for (j = 0; j < n; ++j) {
            for (k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank != 0) {
        for (i = startRow; i < endRow; ++i) {
            MPI_Send(C[i], n, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    } 
    else {
        for (i = 1; i < size; ++i) {
            int rowsToRecv = rowsPerProc;
            if (i == size - 1) {
                rowsToRecv += rowsRemain;
            }

            for (j = 0; j < rowsToRecv; ++j) {
                MPI_Recv(C[startRow + j], n, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) { 
        printf("Time in ms: %f\n", (end - start) * 1000);
    }

    MPI_Finalize();

    return 0;
}
