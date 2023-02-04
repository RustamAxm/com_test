//
// Created by rustam on 01.02.23.
//

#include "Serial_impl.h"

namespace Serial {

    Serial::Serial(const std::string &com) {

        port = open(const_cast<const char *>(com.c_str()), O_RDWR);

        struct termios oldsettings, newsettings;
        if (tcgetattr(port, &oldsettings) != 0) {
            throw std::logic_error("from tcgetattr");
        }
        newsettings = oldsettings;


//        newsettings.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
//        newsettings.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
//        newsettings.c_cflag &= ~CSIZE; // Clear all bits that set the data size
//        newsettings.c_cflag |= CS8; // 8 bits per byte (most common)
//        newsettings.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
//        newsettings.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
//
//        newsettings.c_lflag &= ~ICANON;
//        newsettings.c_lflag &= ~ECHO; // Disable echo
//        newsettings.c_lflag &= ~ECHOE; // Disable erasure
//        newsettings.c_lflag &= ~ECHONL; // Disable new-line echo
//        newsettings.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
//        newsettings.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
//        newsettings.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
//
//        newsettings.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
//        newsettings.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
        // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
        // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

        newsettings.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
        newsettings.c_cc[VMIN] = 0;

        // Set in/out baud rate to be 9600
        cfsetispeed(&newsettings, B115200);
        cfsetospeed(&newsettings, B115200);

        if (tcsetattr(port, TCSANOW, &newsettings) != 0) {
            throw std::logic_error("from tcgetattr");
        }
        tcflush(port, TCIFLUSH);

        if (port <= 0) {
            throw std::logic_error("Port opening failed");
        } else {
            std::cout << "Port " << port << " opened" << std::endl;
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
        data.push_back('\r');
        data.push_back('\n');
        return write(port, const_cast<const char *>(data.c_str()), data.size());
    }

    std::vector<uint16_t> Serial::ReadBin(size_t n_int) const {
        std::vector<uint16_t> buffer;
        buffer.reserve(n_int);
        uint16_t tmp[n_int];
        int ret = read(port, tmp, sizeof(buffer));
        if (ret) {
            for (auto a : tmp) {
                buffer.push_back(a);
            }
        }
        return buffer;
    }

    bool Serial::Close() const {
        return !close(port);
    }

    bool Serial::FillData(std::string & data) const {
        char buffer[512];
        int ret = read(port, buffer,sizeof(buffer));
        if (ret) {
            for (auto ch : buffer) {
                data.push_back(ch);
                if (ch == '\n') {
                    break;
                }
            }
            return true;
        } else {
            return false;
        }
    }

}