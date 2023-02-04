
#include "Serial_impl.h"

void Query(auto &ser, std::string & query_) {
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
};

void test_bin() {
    std::string com = "/dev/ttyUSB0";
    Serial::Serial ser(com);
    sleep(2);

    std::string bin = "start";
    ser.Write(bin);
    std::cout << bin << std::endl;

    for (int i =0; i< 6; ++i) {
        auto data= ser.ReadBin(1000);

        for (auto it : data) {
            std::cout << it << " ";
        }
        std::cout << std::endl;
    }
}

void test_str() {
    std::string com = "/dev/ttyUSB0";
    Serial::Serial ser(com);
    sleep(2);

    std::string start = "start";
    ser << start;
    std::string buffer;
    buffer = ser.Read();
    std::cout << buffer << std::endl;
}

int main() {
    test_bin();
    return 0;

}
