#include <iostream>
#include <vector>
#include <cmath>
#include <fftw3.h>



int main() {
    // Пример: два многочлена
    // A(x) = 1 + 2x + 3x^2
    // B(x) = 2 + x
    std::vector<double> A = {-1, 2, 3, 0, 4 ,7};
    std::vector<double> B = {2, -1.5, 0, 0 , 12.3};

    // Размер FFT: степень результата = deg(A)+deg(B)
    int n = A.size() + B.size() - 1; // степень результата

    // Выделяем память под комплексные массивы
    fftw_complex *fftA = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
    fftw_complex *fftB = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
    fftw_complex *fftC = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);

    // Заполняем входные массивы (imag = 0)
    for (int i = 0; i < n; i++) {
        fftA[i][0] = (i < A.size()) ? A[i] : 0.0;
        fftA[i][1] = 0.0;

        fftB[i][0] = (i < B.size()) ? B[i] : 0.0;
        fftB[i][1] = 0.0;
    }

    // Планируем FFT
    fftw_plan planA = fftw_plan_dft_1d(n, fftA, fftA, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan planB = fftw_plan_dft_1d(n, fftB, fftB, FFTW_FORWARD, FFTW_ESTIMATE);

    // Прямое FFT
    fftw_execute(planA);
    fftw_execute(planB);

    // Покоэффициентное умножение в спектральной области
    for (int i = 0; i < n; i++) {
        double real = fftA[i][0]*fftB[i][0] - fftA[i][1]*fftB[i][1];
        double imag = fftA[i][0]*fftB[i][1] + fftA[i][1]*fftB[i][0];
        fftC[i][0] = real;
        fftC[i][1] = imag;
    }

    // Обратное FFT
    fftw_plan planC = fftw_plan_dft_1d(n, fftC, fftC, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(planC);

    // Выводим результат (нормируем на n)
    std::cout << "Результат перемножения:\n";
    for (int i = 0; i < A.size() + B.size() - 1; i++) {
        double coeff = fftC[i][0] / n; // действительная часть
        std::cout << round(coeff) << "x^" << i << " + "; // округляем до целых
    }

    // Очистка
    fftw_destroy_plan(planA);
    fftw_destroy_plan(planB);
    fftw_destroy_plan(planC);
    fftw_free(fftA);
    fftw_free(fftB);
    fftw_free(fftC);
}
//g++ main.cpp -o main.exe -lfftw3 -lm && ./main.exe