#include <iostream>
#include <thread>
#include <mutex>

int main() {
    int counter = 0;
    std::mutex my_mutex;
    std::thread::id id;
    auto function = [&]   {
        my_mutex.lock();
        id = std::this_thread::get_id();
        counter+=*reinterpret_cast<int*>(&id);
        std::cout << counter << std::endl;
        my_mutex.unlock();
    };
    std::thread t1(function);
    std::thread t2(function);
    std::thread t3(function);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
