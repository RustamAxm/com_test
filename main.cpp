
#include "Serial_impl.h"


int main() {

    std::string message = "Hello world";
    std::string com = "/dev/ttyUSB0";
    Serial::Serial ser(com);
    ser << message;
    std::string returned;
    ser >> returned;
    std::cout << returned << std::endl;
    std::cout << ser.Read() << std::endl;
    return 0;

}
