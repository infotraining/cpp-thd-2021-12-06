#include "joining_thread.hpp"
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <thread>
#include <vector>

using namespace std::literals;

void simple_task()
{
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "simple task: " << i << std::endl;
        std::this_thread::sleep_for(250ms);
    }
}

void background_work(size_t id, const std::string& text, std::chrono::milliseconds delay)
{
    std::cout << "bw#" << id << " has started..." << std::endl;

    for (const auto& c : text)
    {
        std::cout << "bw#" << id << ": " << c << std::endl;

        std::this_thread::sleep_for(delay);
    }

    std::cout << "bw#" << id << " is finished..." << std::endl;
}

class BackgroundWork
{
    const int id_;
    const std::string text_;

public:
    BackgroundWork(int id, std::string text)
        : id_ {id}
        , text_ {std::move(text)}
    {
    }

    void operator()(std::chrono::milliseconds delay) const
    {
        std::cout << "BW#" << id_ << " has started..." << std::endl;

        for (const auto& c : text_)
        {
            std::cout << "BW#" << id_ << ": " << c << std::endl;

            std::this_thread::sleep_for(delay);
        }

        std::cout << "BW#" << id_ << " is finished..." << std::endl;
    }
};

int main()
{
    std::cout << "Main thread starts..." << std::endl;
    const std::string text = "Hello Threads";

    std::thread thd_empty;

    std::cout << "thd_empty.id: " << thd_empty.get_id() << std::endl;

    std::thread thd_1 {&simple_task};
    std::thread thd_2 {[]
        { simple_task(); }};

    // JoiningThread thd_3{&background_work, 1, "text", 250ms};

    const std::string text_content = "multithreading";
    std::thread thd_4 {&background_work, 2, std::cref(text_content), 250ms};

    int id = 3;
    std::thread thd_5 {[&text_content, id]()
        { background_work(id, text_content, 450ms); }};

    thd_1.join();
    thd_2.detach();
    thd_4.join();
    thd_5.join();

    assert(!thd_1.joinable());
    assert(!thd_2.joinable());

    ////////////////////////////////////////////////

    std::vector<int> source = {1, 4, 5, 6, 7, 23, 645, 665, 42};

    std::vector<int> target;
    std::vector<int> backup;

    {
        JoiningThread thd_copy([&source = std::as_const(source), &target]
            { std::copy(begin(source), end(source), std::back_inserter(target)); });
        JoiningThread thd_backup([&source = std::as_const(source), &backup]
            { std::copy(begin(source), end(source), std::back_inserter(backup)); });
    } // implicit join()

    std::cout << "target: ";
    for (const auto& item : target)
        std::cout << item << " ";
    std::cout << std::endl;

    std::cout << "backup: ";
    for (const auto& item : backup)
        std::cout << item << " ";
    std::cout << std::endl;

    std::cout << "Main thread ends..." << std::endl;
}
