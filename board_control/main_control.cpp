#include "board_control.h"
#include <thread>
#include <vector>
#include <memory>
#include <iostream>

int main() {

     // 1. 포트 목록 정의
    // 명시적으로 ID와 포트 매핑
    std::vector<std::pair<int, std::string>> board_info = {
        {1, "/dev/ttyAMA1"}, // board 1 <-> UART3
        {2, "/dev/ttyAMA0"}  // board 2 <-> UART0
    };

    std::vector<std::shared_ptr<BoardController>> boards;
    for (const auto& [id, port] : board_info) {
        boards.push_back(std::make_shared<BoardController>(port, id));
    }

     // 3. 각 보드에 대해 동시에 LCD ON 명령 전송 (멀티스레드)
    std::vector<std::thread> threads;
    for (auto& board : boards) {
        threads.emplace_back([&board]() {
            board->send_lcd_on();  // 동시에 ON
        });
    }

    // 4. 모든 스레드 종료 대기
    for (auto& t : threads) t.join();

    std::cout << "All LCDs turned ON simultaneously!" << std::endl;

    return 0;
}
