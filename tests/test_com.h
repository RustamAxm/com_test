#pragma once

#include "../modules/Serial_impl.h"
#include "../modules/TSQueue.h"

void Query(Serial::Serial &ser, std::string & query_) {
    ser.Write(query_);
    sleep(2);
    std::string returned;
    ser >> returned ;
    std::cout << returned;
}

struct data_ {
    uint16_t A0;
    uint16_t A1;
    uint16_t A2;
    uint16_t A3;
    uint16_t A4;
    uint16_t A5;
};

void test_bin() {

    TSQueue<std::vector<uint16_t>> queue_;
    std::string com = "/dev/ttyUSB0";
    Serial::Serial ser(com, B115200);
    sleep(2);

    std::string bin = "start";
    ser.Write(bin);
    std::cout << bin << std::endl;

    for (int i =0; i < 1024; ++i) {
        auto data= ser.ReadBin(6);
        queue_.push(data);
    }
    std::cout << queue_.size() << std::endl;
    auto tmp = queue_.pop();
    for (auto it : tmp.value()) {
        std::cout << it << std::endl;
    }
    std::cout << queue_.size() << std::endl;
}

void test_str() {
    std::string com = "/dev/ttyUSB0";
    Serial::Serial ser(com, B115200);
    sleep(2);

    std::string start = "start";
    ser << start;
    std::string buffer;
    buffer = ser.Read();
    std::cout << buffer << std::endl;
}
