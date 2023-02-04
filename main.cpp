
#include "Serial_impl.h"

void Query(auto &ser, std::string & query_) {
    ser.Write(query_);
    sleep(2);
    std::string returned;
    ser >> returned ;
    std::cout << returned;
}

int main() {

    std::string com = "/dev/ttyUSB0";
    Serial::Serial ser(com);
    sleep(3);
//
//    std::string s = "fill";
//    Query(ser, s);
//
//    std::string message = "send";
//    Query(ser, message);
//
    std::string bin = "start";
    ser.Write(bin);
    sleep(1);
    for (int i =0; i< 1000; ++i) {
        auto data= ser.ReadBin(3);

        for (auto it : data) {
            std::cout << it << " ";
        }
        std::cout << std::endl;
    }

    return 0;

}
