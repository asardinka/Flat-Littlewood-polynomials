#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fftw3.h>

// Генерация случайного многочлена Литтлвуда длины n
std::vector<int> generate_littlewood(int n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1);

    std::vector<int> poly(n);
    for (int i = 0; i < n; i++) {
        poly[i] = dist(gen) ? 1 : -1;
    }
    return poly;
}

std::vector<int> autocorrelation(const std::vector<int>& poly) {
    int n = poly.size();
    std::vector<int> R(n, 0);

    for (int tau = 0; tau < n; tau++) {
        int sum = 0;
        for (int k = 0; k + tau < n; k++) {
            sum += poly[k] * poly[k + tau];
        }
        R[tau] = sum;
    }
    return R;
}

std::vector<double> autocorrelation_fft(const std::vector<int>& poly) {
    int n = poly.size();
    int n_padded = 1;
    while (n_padded < 2 * n) n_padded <<= 1; // дополняем до степени 2 для скорости FFT

    // Создаем входной массив (double) и массив для FFT
    std::vector<double> x(n_padded, 0.0);
    for (int i = 0; i < n; i++) x[i] = static_cast<double>(poly[i]);

    // Массив комплексных чисел для спектра
    fftw_complex* X = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (n_padded/2 + 1));
    fftw_complex* R = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (n_padded/2 + 1));

    // План прямого FFT
    fftw_plan plan_forward = fftw_plan_dft_r2c_1d(n_padded, x.data(), X, FFTW_ESTIMATE);
    fftw_execute(plan_forward);

    // Вычисляем модуль в квадрат
    for (int i = 0; i < n_padded/2 + 1; i++) {
        double real = X[i][0];
        double imag = X[i][1];
        R[i][0] = real*real + imag*imag; // real part
        R[i][1] = 0.0;                   // imaginary part = 0
    }

    // План обратного FFT
    std::vector<double> r_time(n_padded, 0.0);
    fftw_plan plan_backward = fftw_plan_dft_c2r_1d(n_padded, R, r_time.data(), FFTW_ESTIMATE);
    fftw_execute(plan_backward);

    // нормируем и оставляем только первые n элементов
    std::vector<double> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = r_time[i] / n_padded; // FFTW не нормирует обратно
    }

    // освобождаем память
    fftw_destroy_plan(plan_forward);
    fftw_destroy_plan(plan_backward);
    fftw_free(X);
    fftw_free(R);

    return result;
}


void print_vector(const std::vector<int>& v) {
    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << "\n";
}

int main() {
    int n = 8;
    auto poly = generate_littlewood(n);

    std::cout << "Littlewood polynomial coefficients:\n";
    print_vector(poly);

    auto R_test1 = autocorrelation(poly);
    std::cout << "Autocorrelation:\n";
    print_vector(R_test1);

    auto R_test2 = autocorrelation_fft(poly);
    std::cout << "Autocorrelation:\n";
    print_vector(R_test1);
    return 0;
}



//g++ src/test.cpp -o src/test.exe -lfftw3 -lsndfile -lm && ./src/test.exe