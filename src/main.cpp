#include <cmath>
#include <vector>
#include <iostream>
#include <sndfile.h>

#include "Tester.hpp"
#include "AutoCorrelation.hpp"
#include "LittlewoodPolynomial.hpp"




int main() {

    // auto naive = Autocorrelation<int8_t>::computeNaive(poly);
    // auto fft = Autocorrelation<int8_t>::computeFFT(poly);
    
    for (int i = 1; i<=100; ++i){
        LittlewoodPolynomial poly = LittlewoodPolynomial::randomPolynomial(i);

        Tester tester(10);
        
        std::cout << "Naive autocorrelation test " << std::endl;
        double average_naive = tester(&Autocorrelation<int8_t>::computeNaive, poly); std::cout << std::endl;

        std::cout << "FFT autocorrelation test " << std::endl;
        double average_fft = tester(&Autocorrelation<int8_t>::computeFFT, poly);
    }

    return 0;
}


//g++ src/main.cpp -o src/main.exe -lfftw3 -lsndfile -lm && ./src/main.exe