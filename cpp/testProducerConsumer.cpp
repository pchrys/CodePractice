#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>
#include <vector>
#include <future>
#include <functional>

class tProducerConsumer
{

public:
    tProducerConsumer() { ; }

    void startProducerAndConsumer();

private:
    std::vector<std::thread::id> threads;

    std::condition_variable empty;
    std::condition_variable full;

    std::mutex mtx;

    static const int CAP = 1024 * 10;
    static const int THREAD_NUM = 1024;

    std::function<void()> task1 = [this]() {
        auto id = std::this_thread::get_id();
        while (true)
        {
            std::unique_lock<std::mutex> lg(mtx);

            full.wait(lg, [this]() -> bool { return threads.size() < CAP; });
            threads.push_back(id);
            std::cout << "thread " << id << " push_back: " << id << std::endl;
            empty.notify_all();
        }
    };

    std::function<void()> task2 = [this] {
        auto id = std::this_thread::get_id();
        while (true)
        {
            std::unique_lock<std::mutex> lg(mtx);
            empty.wait(lg, [this]() -> bool { return !threads.empty(); });
            auto v = threads.back();
            threads.pop_back();
            full.notify_all();
            std::cout << "thread " << id << " pops back: " << v << std::endl;
        }
    };
};


void tProducerConsumer::startProducerAndConsumer()
{
    std::vector<std::thread> threads;
    std::vector<std::future<void>> futures;
    for (int i = 0; i < THREAD_NUM; i++)
    {
        auto f1 = std::async(std::launch::async, task1);
        auto f2 = std::async(std::launch::async, task2);

        //threads.emplace_back(task1);
        //threads.emplace_back(task2);

        // futures.push_back(std::async(std::launch::async, task1));
        // futures.push_back(std::async(std::launch::async, task2));

        //futures.push_back(std::async(task1));
        //futures.push_back(std::async(task2));

    }

    for(auto& t: threads){
        t.join();
    }
    // f1.get();
    // f2.get();
}


int main()
{

    tProducerConsumer pc;

    pc.startProducerAndConsumer();

    return 0;
}
