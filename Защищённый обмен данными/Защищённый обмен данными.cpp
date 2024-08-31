#include <iostream>
#include <mutex>
#include <algorithm> // для std::swap

// Класс Data
class Data {
public:
    int value;          // скалярные данные
    std::mutex mtx;     // мьютекс для защиты данных

    Data(int v) : value(v) {}
};

// Вариант 1: swap с использованием std::lock и std::lock_guard
void swap_lock(Data& a, Data& b) {
    std::lock(a.mtx, b.mtx);  // Захват обоих мьютексов
    std::lock_guard<std::mutex> lock1(a.mtx, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(b.mtx, std::adopt_lock);
    std::swap(a.value, b.value);  // Обмен значений
}

// Вариант 2: swap с использованием std::scoped_lock
void swap_scoped_lock(Data& a, Data& b) {
    std::scoped_lock lock(a.mtx, b.mtx);  // Захват обоих мьютексов
    std::swap(a.value, b.value);  // Обмен значений
}

// Вариант 3: swap с использованием std::unique_lock
void swap_unique_lock(Data& a, Data& b) {
    std::unique_lock<std::mutex> lock1(a.mtx, std::defer_lock);
    std::unique_lock<std::mutex> lock2(b.mtx, std::defer_lock);
    std::lock(lock1, lock2);  // Захват обоих мьютексов
    std::swap(a.value, b.value);  // Обмен значений
}

int main() {
    Data data1(1);
    Data data2(2);

    std::cout << "Before swap:" << std::endl;
    std::cout << "Data1: " << data1.value << ", Data2: " << data2.value << std::endl;

    swap_lock(data1, data2);
    std::cout << "After swap_lock:" << std::endl;
    std::cout << "Data1: " << data1.value << ", Data2: " << data2.value << std::endl;

    swap_scoped_lock(data1, data2);
    std::cout << "After swap_scoped_lock:" << std::endl;
    std::cout << "Data1: " << data1.value << ", Data2: " << data2.value << std::endl;

    swap_unique_lock(data1, data2);
    std::cout << "After swap_unique_lock:" << std::endl;
    std::cout << "Data1: " << data1.value << ", Data2: " << data2.value << std::endl;

    return 0;
}
