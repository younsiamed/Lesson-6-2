#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

// Атомарная переменная для количества клиентов
std::atomic<int> client_count(0);

// Максимальное количество клиентов
const int max_clients = 5;

void client_generator() {
    while (true) {
        // Атомарное увеличение счётчика клиентов с relaxed упорядочением
        int expected = client_count.load(std::memory_order_relaxed);
        if (expected < max_clients) {
            client_count.fetch_add(1, std::memory_order_relaxed);
            std::cout << "Клиент добавлен. Количество клиентов: " << client_count.load(std::memory_order_relaxed) << std::endl;
        }
        else {
            std::cout << "Максимальное количество клиентов достигнуто. Ожидание..." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void operator_func() {
    while (true) {
        // Атомарное уменьшение счётчика клиентов с release/acquire упорядочением
        int expected = client_count.load(std::memory_order_acquire);
        if (expected > 0) {
            client_count.fetch_sub(1, std::memory_order_release);
            std::cout << "Клиент обслужен. Количество клиентов: " << client_count.load(std::memory_order_acquire) << std::endl;
        }
        else {
            std::cout << "Клиентов для обслуживания нет. Ожидание новых клиентов..." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

int main() {
    // Создаем потоки для генерации клиентов и работы операциониста
    std::thread client_thread(client_generator);
    std::thread operator_thread(operator_func);

    // Ожидаем завершения потоков (если нужно)
    client_thread.join();
    operator_thread.join();

    return 0;
}
