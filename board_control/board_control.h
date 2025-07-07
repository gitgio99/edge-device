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

    // 확장: ACK/NACK 신뢰성 송신
    bool send_lcd_on_with_ack(int retries = 3, int timeout_ms = 1000);
    bool send_lcd_off_with_ack(int retries = 3, int timeout_ms = 1000);
    
private:
    int fd;
    int id;

    void open_port(const std::string& device);
    void send_frame(uint8_t command);

    // 확장: ACK/NACK 신뢰성 송신
    bool send_frame_with_ack(uint8_t command, int retries, int timeout_ms);

    std::vector<uint8_t> encode_frame(uint8_t command);
};
