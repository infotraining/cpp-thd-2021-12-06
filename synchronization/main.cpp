#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using namespace std::literals;

template <typename T>
struct Synchronized
{
    T value;
    std::mutex mtx_value;
};

template <typename F_, typename Value_>
void apply(F_ f, Synchronized<Value_>& sync_value)
{
    std::lock_guard<std::mutex> lk {sync_value.mtx_value};
    f(sync_value.value);
}

void run(Synchronized<int>& counter)
{
    for (long i = 0; i < 1'000'000; ++i)
    {
        apply([&](int& v) { ++v; }, counter);
    }
}

int main()
{
    std::cout << "Main thread starts..." << std::endl;
    const std::string text = "Hello Threads";

    Synchronized<int> counter;

    std::thread thd1 {run, std::ref(counter)};
    std::thread thd2 {[&counter]
        { run(counter); }};

    thd1.join();
    thd2.join();

    std::cout << "counter: " << counter.value << "\n";

    std::cout << "Main thread ends..." << std::endl;
}
