#pragma once
#include <vector>
#include <cstdint>
#include <iostream>
#include <random>
#include <stdexcept> 

class LittlewoodPolynomial {
public:
    LittlewoodPolynomial() {}
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

    size_t polynomialDegree() const {return degree;}

    int operator[](size_t i) const {
        if (i > degree)
            throw std::out_of_range("Index out of range");
        
        return coefficients[i];
    }

    const std::vector<int8_t>& getCoefficients() const {
        return coefficients;
    }

    static LittlewoodPolynomial randomPolynomial(size_t degree) {
        std::vector<int8_t> coeffs(degree + 1);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 1);

        for (size_t i = 0; i <= degree; ++i) {
            coeffs[i] = dist(gen) ? 1 : -1;
        }

        return LittlewoodPolynomial(coeffs);
    }


    int8_t getCoefficient(size_t i) const {
        return coefficients[i];
    }

    ~LittlewoodPolynomial() {}
    
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