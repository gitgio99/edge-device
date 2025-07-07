#include "board_control.h"
#include <thread>
#include <vector>
#include <memory>
#include <iostream>

int main() {

     // 1. 포트 목록 정의
    // 명시적으로 ID와 포트 매핑
    std::vector<std::pair<int, std::string>> board_info = {
        {1, "/dev/ttyAMA0"}, // board 1 <-> UART0
        {2, "/dev/ttyAMA2"},  // board 2 <-> UART2
        {3, "/dev/ttyAMA1"},  // board 3 <-> UART1
        {4, "/dev/ttyAMA3"}   // board 4 <-> UART3
    };

    std::vector<std::shared_ptr<BoardController>> boards;
    for (const auto& [id, port] : board_info) {
        boards.push_back(std::make_shared<BoardController>(port, id));
    }

    // 3. 각 보드에 대해 동시에 LCD ON 명령 전송 (멀티스레드, ACK/NACK 포함)
    std::vector<std::thread> threads;
    std::vector<bool> results(boards.size(), false);

    for (size_t i = 0; i < boards.size(); ++i) {
        threads.emplace_back([&, i]() {
            results[i] = boards[i]->send_lcd_on_with_ack(3, 1000);  // 3회 재시도, 타임아웃 1초
        });
    }

    // 4. 모든 스레드 종료 대기
    for (auto& t : threads) t.join();

    bool all_ok = true;
    for (size_t i = 0; i < results.size(); ++i) {
        if (!results[i]) {
            std::cout << "[Board " << (i + 1) << "] Error: LCD ON command not acknowledged!" << std::endl;
            all_ok = false;
        }
    }

    if (all_ok)
        std::cout << "All LCDs turned ON and acknowledged!" << std::endl;
    else
        std::cout << "Some LCDs failed to acknowledge the ON command." << std::endl;

    return 0;
}
