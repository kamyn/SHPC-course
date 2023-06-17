#include <iostream>
#include <omp.h>
#include <random>

int main()
{
    intptr_t n = 1e3;
    intptr_t** A = new intptr_t*[n];
    intptr_t** B = new intptr_t*[n];
    intptr_t** C = new intptr_t*[n];

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> rand100(-100,100);

    for (intptr_t i = 0; i < n; ++i) {
        A[i] = new intptr_t[n];
        B[i] = new intptr_t[n];
        C[i] = new intptr_t[n];
    }

    for (intptr_t i = 0; i < n; ++i)
        for (intptr_t j = 0; j < n; ++j) {
            A[i][j] = rand100(rng);
            B[i][j] = rand100(rng);
            C[i][j] = 0;
        }

    double start = omp_get_wtime();
#pragma omp parallel for 
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            for (int k = 0; k < n; ++k){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    double end = omp_get_wtime();

    std::cout << "Time in ms: " << (end - start) * 1000 << '\n';
}
