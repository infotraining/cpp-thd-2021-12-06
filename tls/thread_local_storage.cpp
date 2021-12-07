#include <string>
#include <thread>

#include <cstdlib>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

constexpr int64_t IMUL = 314159269;
constexpr int64_t IADD = 453806245;
constexpr int64_t MASK = 2147483647;
constexpr double SCALE = 0.4656612873e-9;

namespace Unsafe
{
    double rand()
    {
        static int seed = 665; // shared state

        seed = (seed * IMUL + IADD) & MASK;
        return (seed * SCALE);
    }
}

namespace ThreadSafe
{
    double rand_double()
    {
        auto thread_id = std::this_thread::get_id();
        std::hash<std::thread::id> hasher;
        thread_local int thd_seed = 665 * hasher(thread_id);

        int result = (thd_seed * IMUL + IADD) & MASK;
        thd_seed = result;
        return (result * SCALE);
    }
}

int main()
{
    std::thread thd1([] {
        for (size_t i = 0; i < 100; ++i)
            std::cout << ThreadSafe::rand_double() << std::endl;
    });

    std::thread thd2([] {
        for (size_t i = 0; i < 100; ++i)
            std::cout << ThreadSafe::rand_double() << std::endl;
    });

    thd1.join();
    thd2.join();
}
