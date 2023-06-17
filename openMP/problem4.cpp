#include <iostream>
#include <omp.h>
#include <random>

void merge(int* arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }

    for (int j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    delete[] L;
    delete[] R;
}

void mergeSortParallel(int* arr, int l, int r)
{
    int m = l + (r - l) / 2;
    if (l < r)
    {
#pragma omp parallel sections
        { 
# pragma omp section
            mergeSortParallel(arr, l, m);
# pragma omp section
            mergeSortParallel(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

void mergeSort(int* arr, int l, int r)
{
    int m = l + (r - l) / 2;
    if (l < r){
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

int main() {
    int n = 1e6;
    int* arr = new int[n];

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> rand1e4(-100,100);

    for (int i = 0; i < n; ++i)
        arr[i] = rand1e4(rng);

    double start = omp_get_wtime();
    mergeSort(arr, 0, n - 1); 
    double end = omp_get_wtime();
    std::cout << "Time in ms (seq): " << (end - start) * 1000 << '\n';
    
    for (int i = 0; i < n; ++i)
        arr[i] = rand1e4(rng);

    start = omp_get_wtime();
    mergeSortParallel(arr, 0, n-1);
    end = omp_get_wtime();
    std::cout << "Time in ms (parallel): " << (end - start) * 1000 << '\n';


    return 0;
}
