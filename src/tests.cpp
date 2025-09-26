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

    // Временной тест вычисления автокорреляции на случайном  многочлене степени n
    // Рассматриваются степени от 10 до 500 с шагом 10
    // Для каждой степени производится вычисление автокорреляции 100 раз и вычисляется среднее время выполнения в миллисекундах
    // Результаты сохраняются в файл autocorrelation_times.csv

    std::ofstream fout("./files/autocorrelation_times.csv");
    fout << "n,naive,fft\n";

    Tester tester(100);

    for (int n = 10; n <= 500; n += 10) {
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

    // Вычисление автокорреляции для многочлена 1_000_000 степени
    // Результаты сохраняются в файл autocorrelation_result.txt
    // Вычисляется среднее время выполения в миллисекундах на основве 10 запусков
    // Мой результат 243.464 ms

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

void shapiroTest(size_t degree = 255){
    // Построение многочленов Шапиро и случайного многочлена для анализа

    LittlewoodPolynomial r_poly = LittlewoodPolynomial::randomPolynomial(degree);
    LittlewoodPolynomial s_poly = LittlewoodPolynomial::shapiroPolynomial(degree);

    Tester::saveVectorToFile(r_poly.getCoefficients(), "./files/random_poly.txt");
    Tester::saveVectorToFile(s_poly.getCoefficients(), "./files/flat_poly.txt");
}

int main() {

    autocorrelationFileTest();

    return 0;
}

// Запуск
// g++ src/tests.cpp -o src/tests.exe -lfftw3&& ./src/tests.exe

