//
// Created by rustam on 01.02.23.
//

#include "Serial_impl.h"

namespace Serial {

    Serial::Serial(const std::string &com) {
        port = open(const_cast<const char *>(com.c_str()), O_RDWR);
        if (port <= 0) {
            std::cout << "Port opening failed" << std::endl;
        } else {
            std::cout << "Port "<< port << " opened" << std::endl;
        }
    }

    Serial::~Serial() {
        if (Close()) {
            std::cout << "Com port closed" << std::endl;
        } else {
            std::cerr << "Closing error" << std::endl;
        }
    }

    bool Serial::operator <<(std::string & data) {
        return Write(data);
    }

    bool Serial::operator >>(std::string & data) const {
        return Read(data);
    }

    std::string Serial::Read() {
        std::string data;
        data.reserve(512);
        FillData(data);
        return data;
    }

    bool Serial::Read(std::string & data) const {
        return FillData(data);
    }

    bool Serial::Write(std::string & data) {
        data.push_back('\n');
        return write(port, const_cast<const char *>(data.c_str()), data.size());
    }

    bool Serial::Close() const {
        return !close(port);
    }

    bool Serial::FillData(std::string & data) const {
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

}