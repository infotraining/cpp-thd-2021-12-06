#include "catch.hpp"

#include <thread>
#include <iostream>
#include <thread>

using namespace std::literals;

void background_work(const int id, const std::string text, const std::chrono::milliseconds delay)
{
    std::cout << "Thread#" << id << " started..." << std::endl;

    for(const auto& letter : text)
    {
        std::cout << "Thread#" << id << " - " << letter << std::endl;
        std::this_thread::sleep_for(delay);
    }
}

TEST_CASE("running a jthread - thread with RAII")
{
    std::cout << "---------------------------------------------------" << std::endl;

    SECTION("Before C++20")
    {
        std::thread thd1{&background_work, 1, "Hello", 100ms};
        std::thread thd2{&background_work, 2, "Multithreading", 250ms};

        thd1.join();
        thd2.join();
    }

    SECTION("C++20")
    {
        std::jthread jthd1{&background_work, 101, "Hello", 200ms};
        std::jthread jthd2{&background_work, 102, "jthread", 100ms};
    }
}

////////////////////////////////////////////////////////////////////////

void cancelable_background_work(std::stop_token st, const int id, const std::string text, const std::chrono::milliseconds delay)
{
    std::cout << "Thread#" << id << " started..." << std::endl;

    for(const auto& letter : text)
    {
        if (st.stop_requested())
        {
            std::cout << "Stop has been requested..." << std::endl;
            std::cout << "Thread#" << id << " cancelled" << std::endl;
            return;
        }

        std::cout << "Thread#" << id << " - " << letter << std::endl;
        std::this_thread::sleep_for(delay);
    }
}

TEST_CASE("jthread supports cancellation")
{
    std::cout << "---------------------------------------------------" << std::endl;

    SECTION("pass explicitly stop_token")
    {
        std::stop_source stop_src;
        std::stop_token st = stop_src.get_token();

        std::jthread thd1{&cancelable_background_work, st, 1, "FIRSTJOB", 500ms};
        std::jthread thd2{&cancelable_background_work, st, 2, "SECONDJOB", 500ms};
        std::this_thread::sleep_for(2s);
        stop_src.request_stop();
    }

    SECTION("use implicitly stop_token as a first parameter")
    {
        std::jthread thd1{&cancelable_background_work, 1, "FIRSTJOB", 500ms};

        std::stop_source stop_src = thd1.get_stop_source(); // get stop_source from jthread
        std::stop_token st = thd1.get_stop_token();

        std::jthread thd2{&cancelable_background_work, st, 2, "SECONDJOB", 500ms}; // pass stop_token explicitly to another thread
        std::this_thread::sleep_for(2s);
        stop_src.request_stop();
    }
}