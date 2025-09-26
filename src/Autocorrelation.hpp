// Класс для вычисления автокорреляции последовательности
// Имеет два алоритма вычисления - Naive и FFT
// Naive подразумевает попарное умножение всех пар элементов последовательности со сдвигом - сложность O(n^2)
// Алгоритм на основе подразумевает вычисление автокорреляции как произведения многочленов
// Для произведения многочленов используется FFT 
// Для вычисления FFT и обратного преобразования используется библиотека FFTW - сложность O(nlogn)

#pragma once
#include <vector>
#include <stdexcept>
#include <fftw3.h>

template <typename T>
class Autocorrelation
{
public:

    static std::vector<double> computeNaive(const std::vector<T>& seq) {
        if (seq.empty()) {
            throw std::invalid_argument("Sequence must not be empty");
        }

        size_t n = seq.size();
        std::vector<double> result(n, 0.0);

        for (size_t k = 0; k < n; ++k) {
            double sum = 0.0;
            for (size_t i = 0; i + k < n; ++i) {
                sum += static_cast<double>(seq[i]) * static_cast<double>(seq[i + k]);
            }
            result[k] = sum;
        }
        return result;
    }

    static std::vector<double> computeFFT(const std::vector<T>& seq) {
        if (seq.empty()) {
            throw std::invalid_argument("Sequence must not be empty");
        }

        size_t n = seq.size();
        size_t m = 1;
        while (m < 2 * n) m <<= 1;

        std::vector<double> in(m, 0.0);
        for (size_t i = 0; i < n; ++i) {
            in[i] = static_cast<double>(seq[i]);
        }

        fftw_complex* freq = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (m/2 + 1));
        double* out = (double*) fftw_malloc(sizeof(double) * m);

        fftw_plan forward = fftw_plan_dft_r2c_1d(m, in.data(), freq, FFTW_ESTIMATE);
        fftw_plan backward = fftw_plan_dft_c2r_1d(m, freq, out, FFTW_ESTIMATE);

        fftw_execute(forward);

        for (size_t i = 0; i < m/2 + 1; ++i) {
            double re = freq[i][0];
            double im = freq[i][1];
            freq[i][0] = re * re + im * im;
            freq[i][1] = 0.0;
        }

        fftw_execute(backward);

        std::vector<double> result(n);
        for (size_t k = 0; k < n; ++k) {
            result[k] = out[k] / m;
        }

        fftw_destroy_plan(forward);
        fftw_destroy_plan(backward);
        fftw_free(freq);
        fftw_free(out);

        return result;
    }

};
