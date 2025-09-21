#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
#include <sndfile.h>

#include "Tester.hpp"
#include "AutoCorrelation.hpp"
#include "LittlewoodPolynomial.hpp"

void autocorrelationTest() {
    std::ofstream fout("./files/autocorrelation_times.csv");
    fout << "n,naive,fft\n"; // заголовок CSV

    Tester tester(10);

    for (int n = 100; n <= 10000; n += 100) {
        LittlewoodPolynomial poly = LittlewoodPolynomial::randomPolynomial(n);


        std::cout << "Naive autocorrelation test n=" << n << std::endl;
        double average_naive = tester(&Autocorrelation<int8_t>::computeNaive, poly); std::cout << std::endl;

        double average_fft = tester(&Autocorrelation<int8_t>::computeFFT, poly);

        fout << n << "," << average_naive << "," << average_fft << "\n";
    }

    fout.close();
    std::cout << "Data saved to autocorrelation_times.csv\n";
}

int main() {



    return 0;
}
//g++ src/tests.cpp -o src/tests.exe -lfftw3&& ./src/tests.exe

