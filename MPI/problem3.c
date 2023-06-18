#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void merge(int* arr1, int* arr2, int l, int m, int r) {
    int u = l, i = l, j = m + 1, k;
    while (u <= m && j <= r) {
        if (arr1[u] <= arr1[j]) {
            arr2[i] = arr1[u];
            u++;
        } 
        else {
            arr2[i] = arr1[j];
            j++;
        }
        i++;
    }
    if (m < u) {
        for (int k = j; k <= r; k++) {
            arr2[i] = arr1[k];
            i++;
        }
    } 
    else 
        for (int k = u; k <= m; k++) {
            arr2[i] = arr1[k];
            i++;
        }

    for (int k = l; k <= r; k++) 
        arr1[k] = arr2[k];
}

void mergeSort(int* arr1, int* arr2, int l, int r)
{
    int m = l + (r - l) / 2;
    if (l < r){
        mergeSort(arr1, arr2, l, m);
        mergeSort(arr1, arr2, m+1, r);
        merge(arr1, arr2, l, m, r);
    }
}

int main() {	
	int n = 1e7;
	int *arr = malloc(n * sizeof(int));
	
	srand(time(NULL));
	for(int i = 0; i < n; i++) 
		arr[i] = rand() % 201 - 100;
	
    MPI_Init(NULL, NULL);

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    int wsize, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &wsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		
	int size = n/wsize;
	
	int *subArray = malloc(size * sizeof(int));
	MPI_Scatter(arr, size, MPI_INT, subArray, size, MPI_INT, 0, MPI_COMM_WORLD);
	
	int *tmpArray = malloc(size * sizeof(int));
	mergeSort(subArray, tmpArray, 0, (size - 1));
	
	int *sorted = NULL;
	if(rank == 0) 
		sorted = malloc(n * sizeof(int));
	
	MPI_Gather(subArray, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(rank == 0) {
		int *tmpArray2 = malloc(n * sizeof(int));
		mergeSort(sorted, tmpArray2, 0, (n - 1));
	}
	MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();
	MPI_Finalize();

    if (rank == 0)
        printf("Time in ms: %.2f\n", (end - start) * 1000);
}
