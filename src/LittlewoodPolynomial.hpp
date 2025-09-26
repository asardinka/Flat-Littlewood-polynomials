// Класс для работы с полиномами Литтлвуда
// Содержит статические методы для генерации многочленов
// random - генерирует случаный многочлен с коэффициентами 1 или -1
// shapiro - генерирует многочлен Шапиро (для степеней 2^n - 1)

#pragma once
#include <vector>
#include <cstdint>
#include <iostream>
#include <random>
#include <stdexcept> 

class LittlewoodPolynomial {
public:
    LittlewoodPolynomial() {}

    explicit LittlewoodPolynomial(size_t d) : degree(d), coefficients(d + 1, 1) {}
    
    LittlewoodPolynomial(std::vector<int8_t> coeffs) {
        for (auto c : coeffs)
            if (c != 1 && c != -1)
                throw std::invalid_argument("Coefficients must be 1 or -1");

        coefficients = coeffs;
        setDegree();
    }

    LittlewoodPolynomial(std::initializer_list<int8_t> coeffs) {
        for (auto c : coeffs)
            if (c != 1 && c != -1)
                throw std::invalid_argument("Coefficients must be 1 or -1");
        
        coefficients = coeffs;
        setDegree();
    }

    size_t Degree() const {return degree;}
    size_t size() const { return coefficients.size(); }

    auto begin() const { return coefficients.begin(); }
    auto end()   const { return coefficients.end(); }

    int8_t& operator[](size_t i) {
        if (i > coefficients.size())
            throw std::out_of_range("Index out of range");
        
        return coefficients[i];
    }

    const std::vector<int8_t>& getCoefficients() const {
        return coefficients;
    }

    operator const std::vector<int8_t>&() const { return coefficients; }

    static LittlewoodPolynomial randomPolynomial(size_t degree) {
        LittlewoodPolynomial poly(degree);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 1);

        for (size_t i = 0; i <= degree; ++i) {
            poly[i] = dist(gen) ? 1 : -1;
        }

        return poly;
    }

    static LittlewoodPolynomial shapiroPolynomial(size_t degree) {
        
        if (((degree + 1) & degree ) != 0) 
            throw std::invalid_argument("Degree must be a power of 2 - 1");

        LittlewoodPolynomial poly(degree);
        for (size_t k = 0; k <= degree; ++k) {
            size_t x = k;
            int count = 0;
            while (x > 0) {
                if ((x & 3) == 3)
                    count++;
                x >>= 1;
            }
            poly.coefficients[k] = (count % 2 == 0 ? 1 : -1);
        }
        return poly;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const LittlewoodPolynomial& poly);
    

private:
    size_t degree;
    std::vector<int8_t> coefficients;

    void setDegree(){
        degree = coefficients.size() - 1;
    }

};

std::ostream& operator<<(std::ostream& os, const LittlewoodPolynomial& poly) {
    for (size_t i = 0; i <= poly.degree; ++i) {
        int coeff = static_cast<int>(poly.coefficients[i]);
        if (coeff == 0) continue; 
        if (i > 0) {
            if (coeff > 0) os << " + ";
            else os << " - ";
        } else {
            if (coeff < 0) os << "-";
        }

        if (std::abs(coeff) != 1 || i == 0) os << std::abs(coeff);

        if (i >= 1) {
            os << "x";
            if (i >= 2) os << "^" << i;
        }
        
    }
    os << "\n";
    return os;
}