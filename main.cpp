#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string>


class Serial {
public:
    [[maybe_unused]] explicit Serial(const std::string &com) {
        port = open(const_cast<char *>(com.c_str()), O_RDWR);
        if (port <= 0) {
            std::cout << "Port opening failed" << std::endl;
        } else {
            std::cout << "Port "<< port << " opened" << std::endl;
        }
    }

    ~Serial() {
        if (Close()) {
            std::cout << "Com port closed" << std::endl;
        } else {
            std::cerr << "Closing error" << std::endl;
        }
    }

    bool Close() {
        return !close(port);
    }

    bool operator <<(std::string & data) {
        data.push_back('\n');
        return write(port, const_cast<char *>(data.c_str()), data.size());
    }

    bool operator >>(std::string & data) const {
        char buffer[512];
        int ret = read(port, buffer,sizeof(buffer));
        if (ret) {
            for (auto ch : buffer) {
                if (ch == '\n') {
                    break;
                }
                data.push_back(ch);
            }
            return true;
        } else {
            return false;
        }
    }

private:
    int port;
};


int main() {

    std::string message = "Hello world";
    std::string com = "/dev/ttyUSB0";
    Serial ser("/dev/ttyUSB0");
    ser << message;
    std::string returned;
    ser >> returned;
    std::cout << returned << std::endl;
    return 0;

}
