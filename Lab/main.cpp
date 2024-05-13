#include <iostream>
#include <thread>
#include <mutex>

int main() {
    int counter = 0;
    auto function = [&counter] () mutable {
        std::mutex new_mutex;
        new_mutex.lock();
        counter++;
        std::cout << counter << std::endl;
        new_mutex.unlock();
    };
    std::thread t1(function);
    t1.join();
    std::thread t2(function);
    t2.join();
    std::thread t3(function);
    t3.join();
    return 0;
}
