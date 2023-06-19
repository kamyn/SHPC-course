#include <iostream>
#include <omp.h>
#include <time.h>

int main()
{
    intptr_t n = 1e3;
    intptr_t** A = new intptr_t*[n];
    intptr_t** B = new intptr_t*[n];
    intptr_t** C = new intptr_t*[n];

    for (intptr_t i = 0; i < n; ++i) {
        A[i] = new intptr_t[n];
        B[i] = new intptr_t[n];
        C[i] = new intptr_t[n];
    }

    srand(time(NULL));

    for (intptr_t i = 0; i < n; ++i)
        for (intptr_t j = 0; j < n; ++j) {
            A[i][j] = rand() % 201 - 100;
            B[i][j] = rand() % 201 - 100;
            C[i][j] = 0;
        }

    std::cout << "Matrix multiplication report\n";
    std::cout << "-------------------------\n";

    std::cout << "N\tThreads\tTime (ms)\n";

    for (int i = 1; i < 10; i*=2){
        double start = omp_get_wtime();
#pragma omp parallel for num_threads(i)
        for (int i = 0; i < n; ++i){
            for (int j = 0; j < n; ++j){
                for (int k = 0; k < n; ++k){
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        double end = omp_get_wtime();

        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                C[i][j] = 0;

        std::cout << n << '\t' << i << '\t' << (end - start) * 1000 << '\n';
    }
}
