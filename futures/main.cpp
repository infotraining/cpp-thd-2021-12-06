#include <cassert>
#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std::literals;

int calculate_square(int x)
{
    std::cout << "Starting calculation for " << x << " in " << std::this_thread::get_id() << std::endl;

    std::random_device rd;
    std::uniform_int_distribution<> distr(100, 5000);

    std::this_thread::sleep_for(std::chrono::milliseconds(distr(rd)));

    if (x % 3 == 0)
        throw std::runtime_error("Error#3");

    return x * x;
}

void save_to_file(const std::string& filename)
{
    std::cout << "Saving to file: " << filename << std::endl;

    std::this_thread::sleep_for(3s);

    std::cout << "File saved: " << filename << std::endl;
}

////////////////////////////////////////////////////////////
// promise

class SquareCalculator
{
    std::promise<int> promise_;
public:
    void calculate(int x)
    {
        try
        {
            int result = calculate_square(x);
            promise_.set_value(result);
        }
        catch(...)
        {
            promise_.set_exception(std::current_exception());
        }
    }

    std::future<int> get_future()
    {
        return promise_.get_future();
    }
};

void using_promise()
{
    SquareCalculator calc;

    std::future<int> f = calc.get_future();
    std::shared_future<int> shared_f = f.share();

    std::thread consumer_thd{[shared_f] {
        std::cout << "Consuming: " << shared_f.get() << std::endl;
    }};

    std::thread thd_calc{[&] { calc.calculate(11); }};

    std::cout << "11*11 = " << shared_f.get() << std::endl;

    thd_calc.join();
    consumer_thd.join();
}

///////////////////////////////////////////////////////
// packaged_task
void using_packaged_task()
{
    std::packaged_task<int()> pt1{[] { return calculate_square(16);} };

    std::future<int> f1 = pt1.get_future();

    std::thread thd1{std::move(pt1)};
    thd1.detach();

    std::cout << "16 * 16 = " << f1.get() << "\n";
}


template <typename Callable>
auto launch_async(Callable&& callable)
{
    using ResultT = decltype(callable());

    std::packaged_task<ResultT()> pt(std::forward<Callable>(callable));
    std::future<ResultT> f = pt.get_future();

    std::thread thd1{std::move(pt)};
    thd.detach();

    return f;
}

int main()
{
    using_promise();

    using_packaged_task();

    std::future<int> f1 = std::async(std::launch::async, &calculate_square, 7);
    std::future<int> f2 = std::async(&calculate_square, 30);

    while (f1.wait_for(100ms) != std::future_status::ready)
    {
        std::cout << "I still haven't found..." << std::endl;
    }

    std::cout << "f1.get() = " << f1.get() << std::endl;

    try
    {
        int value = f2.get(); // start of deferred
        std::cout << "f2.get() = " << value << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    auto fs1 = std::async(std::launch::async, save_to_file, "file1.txt");
    auto fs2 = std::async(std::launch::async, save_to_file, "file2.txt");
    auto fs3 = std::async(std::launch::async, save_to_file, "file3.txt");
    launch_async([] { save_to_file("file4.txt"); });
    launch_async([] { save_to_file("file5.txt"); });

    std::future<int> fsquare = std::async(std::launch::async, &calculate_square, 13);

    std::shared_future<int> sf = fsquare.share();

    std::thread thd{[sf] { std::cout << sf.get() << " in thread" << std::endl;}};
    auto _ = std::async(std::launch::async, [sf] { std::cout << sf.get() << " in async" << std::endl;});

    thd.join();
}
