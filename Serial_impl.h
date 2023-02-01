# pragma once

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string>

namespace Serial {

    class Serial {
    public:
        Serial(const Serial & other) = delete;
        Serial operator =(const Serial &other) = delete;
        Serial(Serial && other) = delete;
        Serial operator =(Serial && other) = delete;

        explicit Serial(const std::string &com);

        ~Serial();

        bool operator <<(std::string & data);

        bool operator >>(std::string & data) const;

        std::string Read();

        bool Read(std::string & data) const;

        bool Write(std::string & data);

    private:
        int port;

        bool Close() const;

        bool FillData(std::string & data) const;
    };

}

