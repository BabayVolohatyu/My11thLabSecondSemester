#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

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

template<typename T>
struct DefendedQueue {
private:
    std::queue<T> queue;
    std::mutex mutex;
    std::condition_variable condition_variable;
    bool condition_to_read = false;
public:
    void enqueue(const T &data) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            queue.push(data);
            std::cout << "pushed:" << data << std::endl;
        }
        condition_to_read = true;
        condition_variable.notify_one();
    }

    T dequeue() {
        T obj;
        if(queue.empty()) return {};
        {std::unique_lock<std::mutex> lock(mutex);
        //condition_variable.wait(lock, [this] { return !queue.empty(); });
        obj = queue.front();
        queue.pop();
        std::cout << "poped" << std::endl;
        }
        condition_to_read = true;
        condition_variable.notify_one();
        return obj;
    }

    void show() {
        { std::unique_lock<std::mutex> lock(mutex);
        if (queue.empty()) {
            return;
        }
        condition_variable.wait(lock, [this] { return condition_to_read; });
        std::queue<T> q1 = queue;
        while (!q1.empty()) {
            std::cout << " " << q1.front();
            q1.pop();
        }
        std::cout << " read" << std::endl;
    } condition_to_read = false;
    }
};

void task5() {
    DefendedQueue<int> queue;
    auto f1 = [&] {
        for (int i = 0; i < 10; i++) {
            queue.enqueue(9);
            std::cout << "f1:" << i<< std::endl;
            //std::this_thread::sleep_for(std::chrono::milliseconds(100));

        }
    };
    auto f2 = [&] {
        for (int i = 0; i < 11; i++) {
            queue.dequeue();
            std::cout << "f2:" << i<< std::endl;
            //std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
    };
    auto f3 = [&] {
        for (int i = 0; i < 20; i++) {
            queue.show();
            std::cout << "f3:" << i<< std::endl;
            //std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    };
    std::thread t1(f1);
    std::thread t2(f2);
    std::thread t3(f3);
    t1.join();
    t2.join();
    t3.join();
}

int main() {
    //task3();
    //task4();
    task5();

    return 0;
}
