#include <sndfile.h>
#include <vector>
#include <iostream>
#include <cmath>
#include "Tester.hpp"
#include "LittlewoodPolynomial.hpp"




int main() {
    
    LittlewoodPolynomial poly = LittlewoodPolynomial::randomPolynomial(5);

    std::cout << poly[2] << std::endl;

    //Tester tester(5, Tester::TimeUnit::MILLISECONDS);

    //tester([&poly](size_t i) {return poly.getCoefficient(i);}, 1);


    return 0;
}


//g++ src/main.cpp -o src/main.exe -lfftw3 -lsndfile -lm && ./src/main.exe