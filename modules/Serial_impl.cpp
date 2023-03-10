//
// Created by rustam on 01.02.23.
//

#include "Serial_impl.h"

namespace Serial {

    Serial::Serial(const std::string &com) : Serial(com, B115200) {
    }

    Serial::Serial(const std::string &com, speed_t speed) {

        port = open(const_cast<const char *>(com.c_str()), O_RDWR);

        struct termios oldsettings, newsettings;
        if (tcgetattr(port, &oldsettings) != 0) {
            throw std::logic_error("from tcgetattr");
        }
        newsettings = oldsettings;

        newsettings.c_cflag = (newsettings.c_cflag & ~CSIZE) | CS8;
        newsettings.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
        newsettings.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
        newsettings.c_cflag &= ~CSIZE; // Clear all bits that set the data size
        newsettings.c_cflag |= CS8; // 8 bits per byte (most common)
        newsettings.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
        newsettings.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

        newsettings.c_lflag &= ~ICANON;
        newsettings.c_lflag &= ~ECHO; // Disable echo
        newsettings.c_lflag &= ~ECHOE; // Disable erasure
        newsettings.c_lflag &= ~ECHONL; // Disable new-line echo
        newsettings.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
        newsettings.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
        newsettings.c_iflag &= ~IGNBRK; // Disable any special handling of received bytes

//        newsettings.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
//        newsettings.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
//         tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
//         tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

        newsettings.c_cc[VTIME] = 1;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
        newsettings.c_cc[VMIN] = 0;

        // Set in/out baud rate to be 9600
        cfsetispeed(&newsettings, speed);
        cfsetospeed(&newsettings, speed);

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
        uint16_t tmp;
        while (buffer.size() < n_int) {
            int ret = read(port, &tmp, sizeof(tmp));
            if (ret) {
                buffer.push_back(tmp);
            }
        }
        return buffer;
    }

    bool Serial::Close() const {
        return !close(port);
    }

    bool Serial::FillData(std::string & data) const {
        char ch;
        while (ch != '\n') {
            int ret = read(port, &ch,sizeof(ch));
            if (ret) {
                data.push_back(ch);
            }
        }
        return true;
    }

}