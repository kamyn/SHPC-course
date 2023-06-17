#include <iostream>
#include <omp.h>

int main() {
    double times[12];
    for (int i = 0; i < 12; ++i)
        times[i] = 0.;

    int nt[] = {1,2,4,8,10,12};
    intptr_t n = 1e2;
    double start = 0., stop = 0.;
    double pi = .0;

    for (int i = 0; i < 12; ++i){
        pi = 0.;
        n = i % 2 == 0 ? 1e2 : 1e6;
        start = omp_get_wtime();
        #pragma omp parallel for num_threads(nt[i / 2]) reduction(+:pi)
        for (intptr_t j = 0; j < n; ++j)
            pi += 4. / (1 + ((j + 0.5) / n) * ((j + 0.5) / n));
        stop = omp_get_wtime();
        times[i] = (stop - start) * 1000;
    }
    std::cout << "N\t" << "Threads\t" << "Time ms\t\n";
    for (int i = 0; i < 12; ++i)
        std::cout << (i % 2 == 0 ? 1e2 : 1e6) << '\t' << nt[i/2] << '\t' << times[i] << '\n';
    return 0;
}
