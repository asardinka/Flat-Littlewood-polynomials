#include <cmath>
#include <vector>
#include <iostream>
#include <sndfile.h>

#include "Tester.hpp"
#include "AutoCorrelation.hpp"
#include "LittlewoodPolynomial.hpp"

using namespace std;

template <typename T>
ostream &operator<<(ostream &os, const vector<T> vec){
    for (auto i : vec)
        os << i << " ";
    cout << endl;
    return os;
}

int main() {
    

    LittlewoodPolynomial poly = {1, 1, 1, -1, -1};

    auto autocorr = Autocorrelation<int8_t>::computeFFT(poly);
    
    cout << "Autocorrelation: " << autocorr;
    




    return 0;
}


//g++ src/main.cpp -o src/main.exe -lfftw3 -lsndfile -lm && ./src/main.exe