#include <iostream>
#include <iomanip>
#include <omp.h>

int main() {
    intptr_t n = 1e8;
    intptr_t sum = 0;
    double pi = .0;

#pragma omp parallel for reduction(+:pi)
    for (intptr_t i = 0; i < n; ++i)
        pi += 4. / (1 + ((i + 0.5) / n) * ((i + 0.5) / n));

    std::cout << std::setprecision(9) << "pi: " << pi/n << std::endl;
    return 0;
}
