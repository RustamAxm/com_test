//
// Created by rustam on 05.02.23.
//
#pragma once

#include "../modules/FFT_class.h"
#include "../modules/Serial_impl.h"
#include "../modules/TSQueue.h"
#include <thread>
#include <atomic>
#include <fstream>
#include <chrono>

std::ostream & print_fft_data(std::ostream & os, const std::vector<std::vector<double>> & fft_data) {
    for(const auto & x : fft_data) {
        for (auto y : x) {
            os << y << " ";
        }
        os << std::endl;
    }
    return os;
}

void save_to_txt_data(std::string name, std::vector<std::vector<double>> & data) {
    std::ofstream outfile;
    outfile.open(name, std::ios_base::app);//std::ios_base::app
    print_fft_data(outfile, data);
    std::cout << "data saved" << std::endl;
}


int test_com_fft() {

    std::atomic_bool runing = true;
    std::atomic_size_t period; // micro

    TSQueue<std::vector<uint16_t>> queue_;
    std::vector<std::vector<double>> fft_data;
    size_t counts_len = 2048;
    int struct_len = 6;

    for (int i = 0; i < struct_len; ++i) {
        std::vector<double> tmp(counts_len);
        fft_data.push_back(tmp);
    }

    std::string com = "/dev/ttyUSB0";
    Serial::Serial ser(com, B1000000);
    sleep(2);

    std::thread t1([&]() {
        std::cout << "start arduino thread" << std::endl;
        using Clock = std::chrono::steady_clock;
        while (runing) {
            std::string bin = "start";
            ser.Write(bin);
            const Clock::time_point start_time_ = Clock::now();
            for (int i = 0; i < counts_len; ++i) {
                auto data= ser.ReadBin(struct_len);
                queue_.push(data);
            }
            const auto end_time = Clock::now();
            const auto dur = end_time - start_time_;
            period = duration_cast<std::chrono::milliseconds>(dur).count()/counts_len;
        }
    });

    std::thread t2 ([&]() {
        std::cout << "start fft thread" << std::endl;
        while (runing) {
            std::vector<std::vector<uint16_t>> to_fft;
            // init zeros vector<vector>
            for (int i = 0; i < struct_len; ++i) {
                std::vector<uint16_t> tmp(counts_len);
                to_fft.push_back(tmp);
            }
            // fill vector
            for (int i = 0; i < counts_len;) {
                auto tmp = queue_.pop();
                if (tmp != std::nullopt) {
                    for (size_t j = 0; j < tmp->size(); ++j) {
                        to_fft[j][i] = (tmp.value()[j]);
                    }
                    ++i;
                }
            }

            //  accumulate FFT
            for (size_t i = 0; i < to_fft.size(); ++i) {
                class_FFT fft(to_fft[i]);
                auto tmp = fft.get_real();
                std::transform(fft_data[i].begin(), fft_data[i].end(),
                               tmp.begin(), fft_data[i].begin(), std::plus<>());
            }
        }
    });

    char ch;
    while (true) {
        std::cout << "press q to stop" << std::endl;
        std::cin >> ch;
        if (ch == 'q') {
            std::cout<< "q pressed" << std::endl;
            runing = false;
            t1.join();
            t2.join();
            std::cout << "freq = " <<  1.0/(static_cast<double>(period)) << " kHz" << std::endl;
            print_fft_data( std::cout, fft_data);
            save_to_txt_data("data.txt", fft_data);
            return 0;
        }
    }
}
