#include <iostream>
#include <omp.h>

int main() {
    double times[] = {0., 0., 0.};
    int iterations = 30;
    double start = 0., stop = 0.;

    intptr_t n = 1e6;
    intptr_t sum = 0;
    intptr_t* A = new intptr_t[n];

    for(intptr_t i = 0; i < n; ++i)
        A[i] = i;

    // sequential execution
    for (int i = 0; i < iterations; ++i){
        sum = 0;
        start = omp_get_wtime();
        for (intptr_t j = 0; j < n; ++j)
            sum += A[j];
        stop = omp_get_wtime();
        times[0] += stop - start;
    }
    times[0] /= iterations / 1000.;

    // atomic execution
    for (int i = 0; i < iterations; ++i){
        sum = 0;
        start = omp_get_wtime();
        #pragma omp parallel for shared(sum)
        for (intptr_t j = 0; j < n; ++j){
            #pragma omp atomic
            sum += A[j];
        }
        stop = omp_get_wtime();
        times[1] += stop - start;
    }
    times[1] /= iterations / 1000.;

    // reduction execution
    for (int i = 0; i < iterations; ++i){
        sum = 0;
        start = omp_get_wtime();
        #pragma omp parallel for reduction(+:sum)
        for(intptr_t i = 0; i < n; ++i)
            sum += A[i];
        stop = omp_get_wtime();
        times[2] += stop - start;
    } 
    times[2] /= iterations / 1000.;

    std::cout << "Type\t" << "N\t" << "Time ms\t\n";
    std::cout << "Seq\t" << n << '\t' << times[0]<< '\t' << '\n';
    std::cout << "Atomic\t" << n << '\t' << times[1] << '\t' << '\n';
    std::cout << "Reduct\t" << n << '\t' << times[2] << '\t' << '\n';
    return 0;
}

