#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <random>
#include <chrono>
#include <windows.h>

std::mutex console_mutex;

void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void draw_progress(int thread_num, int length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(50, 200);

    auto start = std::chrono::steady_clock::now();
    DWORD tid = GetCurrentThreadId();

    {
        std::lock_guard<std::mutex> lock(console_mutex);
        setCursorPosition(0, thread_num + 1);
        std::cout << thread_num;
        setCursorPosition(5, thread_num + 1);
        std::cout << tid;
    }

    for (int i = 0; i < length; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(distr(gen)));

        {
            std::lock_guard<std::mutex> lock(console_mutex);
            setCursorPosition(15 + i, thread_num + 1);
            std::cout << "█";
            std::cout.flush();
        }
    }

    auto end = std::chrono::steady_clock::now();
    auto sec = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

    {
        std::lock_guard<std::mutex> lock(console_mutex);
        setCursorPosition(50, thread_num + 1);
        std::cout << sec << "s";
    }
}

int main() {
    int num_threads = 5;
    int length = 30;

    // Заголовок таблицы
    std::cout << "#     id          Progress Bar                      Time\n";

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(draw_progress, i, length);
    }

    for (auto& th : threads) {
        th.join();
    }

    setCursorPosition(0, num_threads + 3);
    std::cout << "All done.\n";
}
