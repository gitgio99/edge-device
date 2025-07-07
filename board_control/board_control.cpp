#include "board_control.h"
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <iostream>

#define DLE 0x10
#define STX 0x02
#define ETX 0x03
#define CMD_LCD_ON  0x01
#define CMD_LCD_OFF 0x02

uint8_t reverse(uint8_t val, int bits);
uint16_t crc16(const std::vector<uint8_t>& data);
uint16_t reverse16(uint16_t val, int bits);

BoardController::BoardController(const std::string& device, int board_id)
    : id(board_id)
{
    open_port(device);
}

BoardController::~BoardController() {
    close(fd);
}

void BoardController::open_port(const std::string& device) {
    fd = open(device.c_str(), O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    termios tty{};
    tcgetattr(fd, &tty);
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    tcsetattr(fd, TCSANOW, &tty);
}

void BoardController::send_lcd_on() {
    send_frame(CMD_LCD_ON);
}

void BoardController::send_lcd_off() {
    send_frame(CMD_LCD_OFF);
}

void BoardController::send_frame(uint8_t command) {
    auto frame = encode_frame(command);
    write(fd, frame.data(), frame.size());
}

std::vector<uint8_t> BoardController::encode_frame(uint8_t command) {
    uint8_t dst_mask = 1 << (id - 1);
    std::vector<uint8_t> payload = { dst_mask, command };
    uint16_t crc = crc16(payload);
    payload.push_back((crc >> 8) & 0xFF);
    payload.push_back(crc & 0xFF);

    std::vector<uint8_t> frame = { DLE, STX };
    for (auto b : payload) {
        if (b == DLE) frame.insert(frame.end(), { DLE, DLE });
        else frame.push_back(b);
    }
    frame.push_back(DLE);
    frame.push_back(ETX);
    return frame;
}

// --- CRC Functions ---
uint8_t reverse(uint8_t val, int bits) {
    uint8_t res = 0;
    for (int i = 0; i < bits; ++i)
        res = (res << 1) | ((val >> i) & 1);
    return res;
}

uint16_t reverse16(uint16_t val, int bits) {
    uint16_t res = 0;
    for (int i = 0; i < bits; ++i)
        res = (res << 1) | ((val >> i) & 1);
    return res;
}

uint16_t crc16(const std::vector<uint8_t>& data) {
    uint16_t crc = 0;
    for (auto b : data) {
        crc ^= reverse(b, 8) << 8;
        for (int i = 0; i < 8; ++i)
            crc = (crc & 0x8000) ? (crc << 1) ^ 0x8005 : crc << 1;
    }
    return reverse16(crc, 16) & 0xFFFF;
}
