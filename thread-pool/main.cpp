#include "thread_safe_queue.hpp"
#include "function_wrapper.hpp"
#include <cassert>
#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std::literals;

void background_work(size_t id, const std::string& text, std::chrono::milliseconds delay)
{
    std::cout << "bw#" << id << " has started in the thread#" << std::this_thread::get_id() << std::endl;

    for (const auto& c : text)
    {
        std::cout << "bw#" << id << ": " << c << " - thread#" << std::this_thread::get_id() << std::endl;

        std::this_thread::sleep_for(delay);
    }

    std::cout << "bw#" << id << " is finished..." << std::endl;
}

using Task = StdExtensions::Function;

void using_function()
{
    StdExtensions::Function f1 = [](){ std::cout << "Works\n"; };
    f1();

    f1 = [](){ std::cout << "!!!\n"; };
    f1();
}

// namespace ver_1_0
// {
//     class ThreadPool
//     {
//     public:
//         static inline Task poisoning_pill {};

//         ThreadPool(size_t size)
//             : threads_(size)
//         {
//             for (auto& thd : threads_)
//                 thd = std::thread {[this]
//                     { run(); }};
//         }

//         ~ThreadPool()
//         {
//             for (size_t i = 0; i < threads_.size(); ++i)
//                 send_poisoning_pill();

//             for (auto& thr : threads_)
//             {
//                 if (thr.joinable())
//                     thr.join();
//             }
//         }

//         void submit(const Task& task)
//         {
//             if (task == nullptr)
//                 throw std::invalid_argument("Empty function is not supported");
//             q_tasks_.push(task);
//         }

//     private:
//         void send_poisoning_pill()
//         {
//             q_tasks_.push(poisoning_pill);
//         }

//         void run()
//         {
//             Task task;
//             while (true)
//             {
//                 q_tasks_.pop(task);
//                 if (task == nullptr)
//                     return;

//                 task();
//             }
//         }

//         std::vector<std::thread> threads_;
//         ThreadSafeQueue<Task> q_tasks_;
//     };
// }

namespace ver_1_1
{
    class ThreadPool
    {
    public:
        ThreadPool(size_t size)
            : threads_(size)
        {
            for (auto& thd : threads_)
                thd = std::thread {[this]
                    { run(); }};
        }

        template <typename Callable>
        auto submit(Callable&& task)
        {
            using ResultT = decltype(task());

            auto pt = std::packaged_task<ResultT()>(std::forward<Callable>(task));
            auto fresult = pt.get_future();

            q_tasks_.push( std::move(pt));

            // auto pt = std::make_shared<std::packaged_task<ResultT()>>(std::forward<Callable>(task));
            // auto fresult = pt->get_future();
            // q_tasks_.push([pt] { (*pt)();});

            return fresult;
        }

        ~ThreadPool()
        {
            for (size_t i = 0; i < threads_.size(); ++i)
            {
                submit([this]()
                    { end_work_ = true; });
            }

            for (auto& thr : threads_)
            {
                if (thr.joinable())
                    thr.join();
            }
        }

    private:
        void run()
        {
            Task task;
            while (true)
            {
                q_tasks_.pop(task);
                task();

                if (end_work_)
                    return;
            }
        }

        std::vector<std::thread> threads_;
        ThreadSafeQueue<Task> q_tasks_;
        std::atomic<bool> end_work_ {false};
    };
}

int calculate_square(int x)
{
    return x * x;
}

int main()
{
    std::cout << "Main thread starts..." << std::endl;
    const std::string text = "Hello Threads";

    using namespace ver_1_1;

    ThreadPool thread_pool(6);

    thread_pool.submit([&]
        { background_work(1, text, 20ms); });
    thread_pool.submit([&]
        { background_work(2, "thread pool", 30ms); });
    thread_pool.submit([&]
        { background_work(3, "thread pool", 30ms); });

    std::future<int> fresult = thread_pool.submit([]
        { return calculate_square(48); });

    std::vector<std::future<int>> fsquares;

    for(int i = 10; i < 30; ++i)
        fsquares.push_back(thread_pool.submit([i] { return calculate_square(i); }));

    fsquares.push_back(std::move(fresult));

    for(auto& fs : fsquares)
        std::cout << fs.get() << "\n";

    std::cout << "Main thread ends..." << std::endl;
}
