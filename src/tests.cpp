#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
#include <sndfile.h>

#include "Tester.hpp"
#include "AutoCorrelation.hpp"
#include "LittlewoodPolynomial.hpp"

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> vec){
    for (auto i : vec)
        os << i << " ";
    std::cout << std::endl;
    return os;
}

void autocorrelationTest() {
    std::ofstream fout("./files/autocorrelation_times.csv");
    fout << "n,naive,fft\n";

    Tester tester(10);

    for (int n = 100; n <= 10000; n += 100) {
        LittlewoodPolynomial poly = LittlewoodPolynomial::randomPolynomial(n);


        std::cout << "Naive autocorrelation test n=" << n << std::endl;
        double average_naive = tester(&Autocorrelation<int8_t>::computeNaive, poly); std::cout << std::endl;

        auto dummy = Autocorrelation<int8_t>::computeFFT(poly);
        double average_fft = tester(&Autocorrelation<int8_t>::computeFFT, poly);

        fout << n << "," << average_naive << "," << average_fft << "\n";
    }

    fout.close();
    std::cout << "Data saved to autocorrelation_times.csv\n";
}

void autocorrelationFileTest() {
    std::ifstream fin("./files/coeffs.txt");
    if (!fin) throw std::runtime_error("Cannot open file");

    size_t n;
    fin >> n;

    std::vector<int8_t> coeffs;
    coeffs.reserve(n);

    int value;
    for (size_t i = 0; i < n; ++i) {
        if (!(fin >> value)) throw std::runtime_error("Unexpected end of file or invalid value");
        if (value != 1 && value != -1) throw std::runtime_error("Invalid coefficient in file");
        coeffs.push_back(static_cast<int8_t>(value));
    }

    LittlewoodPolynomial poly(coeffs);
    fin.close();
    
    std::vector<double> result = Autocorrelation<int8_t>::computeFFT(poly);

    Tester tester(10);
    double average_fft = tester(&Autocorrelation<int8_t>::computeFFT, poly);

    std::ofstream fout("./files/autocorrelation_result.txt");
    for (auto r : result) {
        fout << r << " ";
    }
    fout.close();
}

int main() {
    size_t degree = 15;

    LittlewoodPolynomial r_poly = LittlewoodPolynomial::randomPolynomial(degree);
    LittlewoodPolynomial f_poly = LittlewoodPolynomial::flatPolynomial(degree);

    Tester::saveVectorToFile(r_poly.getCoefficients(), "./files/random_poly.txt");
    Tester::saveVectorToFile(f_poly.getCoefficients(), "./files/flat_poly.txt");

    std::cout << "Random polynomial: " << r_poly << std::endl;
    std::cout << "auto correlation for random polynomial: " << Autocorrelation<int8_t>::computeFFT(r_poly) << std::endl << std::endl;

    std::cout << "Flat polynomial: " << f_poly << std::endl;
    std::cout << "auto correlation for flat polynomial: " << Autocorrelation<int8_t>::computeFFT(f_poly) << std::endl << std::endl;


    return 0;
}
//g++ src/tests.cpp -o src/tests.exe -lfftw3&& ./src/tests.exe

