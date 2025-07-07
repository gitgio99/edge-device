#pragma once
#include <vector>
#include <string>
#include <cstdint>

class BoardController {
public:
    BoardController(const std::string& device, int board_id);
    ~BoardController();

    void send_lcd_on();
    void send_lcd_off();

private:
    int fd;
    int id;

    void open_port(const std::string& device);
    void send_frame(uint8_t command);
    std::vector<uint8_t> encode_frame(uint8_t command);
};
