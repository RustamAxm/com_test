//
// Created by rustam on 28.04.22.
//

#include "FFT_class.h"

class_FFT::class_FFT(CArray& input) noexcept {
    size_ = input.size();
    temp_array_ = input;
    count_FFT(temp_array_);
}

CArray class_FFT::get_FFT() {
    return temp_array_;
}

std::vector<double> class_FFT::get_real() {
    std::vector<double> tmp;
    tmp.reserve(size_);
    for (size_t i = 0; i != size_; ++i) {
        tmp.push_back(temp_array_[i].real());
    }
    return tmp;
}

std::vector<double> class_FFT::get_img() {
    std::vector<double> tmp;
    tmp.reserve(size_);
    for (size_t i = 0; i != size_; ++i) {
        tmp.push_back(temp_array_[i].imag());
    }
    return tmp;
}

void class_FFT::count_FFT(CArray& data) {
    const size_t N = data.size();
    if (N <= 1) {
        return;
    }

    CArray even = data[std::slice(0, N/2, 2)];
    CArray odd = data[std::slice(1, N/2, 2)];

    count_FFT(even);
    count_FFT(odd);

    for (size_t i = 0; i < N/2; ++i) {
        Complex tmp = std::polar(1.0, -2 * PI * i / N) * odd[i];
        data[i] = even[i] + tmp;
        data[i + N/2] = even[i] - tmp;
    }
}
