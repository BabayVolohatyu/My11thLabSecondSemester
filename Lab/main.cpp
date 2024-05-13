#include <iostream>
#include <thread>
#include <mutex>

void task3() {
    int counter = 0;
    std::mutex my_mutex;
    std::thread::id id;
    auto function = [&] {
        my_mutex.lock();
        id = std::this_thread::get_id();
        counter += *reinterpret_cast<int *>(&id);
        std::cout << counter << std::endl;
        my_mutex.unlock();
    };
    std::thread t1(function);
    std::thread t2(function);
    std::thread t3(function);
    t1.join();
    t2.join();
    t3.join();
}

void task4() {
    std::mutex mutex1;
    std::mutex mutex2;
    std::thread::id id;
    auto f1 = [&] {
        std::lock_guard<std::mutex> lock1(mutex1);
        std::lock_guard<std::mutex> lock2(mutex2);
        id = std::this_thread::get_id();
        std::cout << "Thread #" << id << " is running" << std::endl;
    };
    auto f2 = [&] {
        std::lock_guard<std::mutex> lock1(mutex1);
        std::lock_guard<std::mutex> lock2(mutex2);
        id = std::this_thread::get_id();
        std::cout << "Thread #" << id << " is running" << std::endl;

    };
    std::thread t1(f1);
    std::thread t2(f2);
    t1.join();
    t2.join();
}

void task5() {

}

int main() {
    //task3();
    task4();
    return 0;
}
