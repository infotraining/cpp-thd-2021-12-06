#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <numeric>
#include <atomic>

using namespace std::literals;

class Data
{
    std::vector<int> data_;
    std::atomic<bool> data_ready_{false};

public:
    void read()
    {
        std::cout << "Start reading..." << std::endl;
        data_.resize(100);

        std::random_device rnd;
        std::generate(begin(data_), end(data_), [&rnd]
            { return rnd() % 1000; });
        std::this_thread::sleep_for(2s);
        std::cout << "End reading..." << std::endl;        
        
        data_ready_.store(true, std::memory_order_release);
        //data_ready_ = true; // -> data_ready_.store(true, std::memory_order_seq_cst); XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    }

    void process(int id)
    {       
        while(!data_ready_.load(std::memory_order_acquire)) // while(!data_ready_.load(std::memory_order_seq_cst)) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        {}
     
        long sum = std::accumulate(begin(data_), end(data_), 0L);

        std::cout << "Id: " << id << "; Sum: " << sum << std::endl;
    }
};

int main()
{
    std::cout << "Main thread starts..." << std::endl;
    const std::string text = "Hello Threads";

    Data data;

    std::thread thd_producer {[&data]
        { data.read(); }};
    
    std::thread thd_consumer1 {[&data]
        { data.process(1); }};
    std::thread thd_consumer2 {[&data]
        { data.process(2); }};

    thd_producer.join();
    thd_consumer1.join();
    thd_consumer2.join();

    std::cout << "Main thread ends..." << std::endl;
}
