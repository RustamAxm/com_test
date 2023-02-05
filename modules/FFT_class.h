//
// Created by rustam on 28.04.22.
//
#pragma once

#include <complex>
#include <valarray>
#include <execution>
#include <cmath>



typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;


class class_FFT {
public:

    explicit class_FFT(CArray& input) noexcept;

    template<typename Container>
    explicit class_FFT(Container & input) noexcept;

    CArray get_FFT();

    std::vector<double> get_real();

    std::vector<double> get_img();

private:
    CArray temp_array_;
    size_t size_ = 0;
    const double PI = 3.14159265;

    void count_FFT(CArray& data);
};

template<typename Container>
class_FFT::class_FFT(Container & input) noexcept {
    size_ = input.size();
    Complex test[size_];
    for (size_t i = 0; i < size_; ++i) {
        test[i] = input[i];
    }
    CArray temp_array_1 (test, size_);
    temp_array_ = temp_array_1;
    count_FFT(temp_array_);
}
