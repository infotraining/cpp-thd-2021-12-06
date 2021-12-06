#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <sstream>

using namespace std::literals;

template <typename T>
struct ThreadResult
{
    T value;
    std::exception_ptr eptr;
};

void background_work(size_t id, const std::string& text, std::chrono::milliseconds delay, ThreadResult<char>& result)
{
    std::ostringstream ss;
    ss << "bw#" << id << " has started..." << std::endl;

    std::cout << ss.str();

    for (const auto& c : text)
    {
        std::cout << "bw#" << id << ": " << c << std::endl;

        std::this_thread::sleep_for(delay);
    }

    try
    {
        result.value = text.at(3);
    }
    catch (...)
    {
        result.eptr = std::current_exception();
        return;
    }

    std::cout << "bw#" << id << " is finished..." << std::endl;
}

int main()
{
    std::cout << "Main thread starts..." << std::endl;
    const std::string text = "Hello Threads";

    const auto no_of_cores = std::max(1u, std::thread::hardware_concurrency());
    std::vector<ThreadResult<char>> results(no_of_cores);
    std::vector<std::thread> thds(no_of_cores);

    for (size_t i = 0; i < results.size(); ++i)
        thds[i] = std::thread {&background_work, i+1, "Text", 100ms, std::ref(results[i])};

    for (auto& thd : thds)
        thd.join();

    for (auto& result : results)
    {
        try
        {
            if (result.eptr)
            {
                std::rethrow_exception(result.eptr);
            }
            else
            {
                std::cout << "Result: " << result.value << "\n";
            }
        }
        catch (const std::out_of_range& e)
        {
            std::cout << "CAUGHT OutOfRange: " << e.what() << "\n";
        }
        catch (const std::exception& e)
        {
            std::cout << "CAUGHT: " << e.what() << "\n";
        }
    }

    std::cout << "Main thread ends..." << std::endl;
}
