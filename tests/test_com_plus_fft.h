//
// Created by rustam on 05.02.23.
//
#pragma once

#include "../modules/FFT_class.h"
#include "../modules/Serial_impl.h"
#include "../modules/TSQueue.h"
#include <thread>
#include <atomic>

int test_com_fft() {
    std::atomic_bool runing = true;

    TSQueue<std::vector<uint16_t>> queue_;
    std::string com = "/dev/ttyUSB0";
    Serial::Serial ser(com);
    sleep(2);

    std::thread t1([&]() {
        std::cout << "start arduino thread" << std::endl;
        while (runing) {
            std::string bin = "start";
            ser.Write(bin);
            for (int i =0; i < 1024; ++i) {
                auto data= ser.ReadBin(6);
                queue_.push(data);
            }
        }
    });

    std::thread t2 ([&]() {
        std::cout << "start fft thread" << std::endl;
        while (runing) {
            std::vector<uint16_t> to_fft;
            to_fft.reserve(1024);

            for (int i = 0; i < 1024;) {
                auto tmp = queue_.pop();
                if (tmp != std::nullopt) {
                    to_fft.push_back(tmp.value()[0]);
                    ++i;
                }
            }
//            std::cout << "queue size = " << queue_.size() << std::endl;
//            std::cout << "to fft vector = " << to_fft.size() << std::endl;

            //  count FFT
            class_FFT fft(to_fft);
            auto fft_data = fft.get_real();
//            for (auto it : fft_data) {
//                std::cout << it << " ";
//            }
//            std::cout << std::endl;
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
            return 0;
        }
    }
}
