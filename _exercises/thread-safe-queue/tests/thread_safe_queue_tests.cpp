#include <chrono>
#include <condition_variable>
#include <queue>
#include <thread>

#include "catch.hpp"

#include "thread_safe_queue.hpp"

using namespace std;

TEST_CASE("ThreadSafeQueue")
{
    ThreadSafeQueue<int> tsq;

    SECTION("is empty after creation")
    {
        REQUIRE(tsq.empty() == true);
    }

    SECTION("is not empty after push")
    {
        tsq.push(1);

        REQUIRE(tsq.empty() == false);
    }

    SECTION("pops items in FIFO order")
    {
        tsq.push(1);
        tsq.push(2);

        int item;
        auto result = tsq.try_pop(item);

        REQUIRE(result);
        REQUIRE(item == 1);
    }

    SECTION("try_pop returns false when last item removed")
    {
        tsq.push(1);

        int item;
        tsq.try_pop(item);
        auto result = tsq.try_pop(item);

        REQUIRE(result == false);
        REQUIRE(tsq.empty() == true);
    }

    SECTION("client waits when poping from empty")
    {
        int item;

        chrono::high_resolution_clock::time_point t1;

        thread thd{[&tsq, &item, &t1] {
            tsq.pop(item);
            t1 = chrono::high_resolution_clock::now();
        }};

        this_thread::sleep_for(200ms);
        chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
        tsq.push(1);
        thd.join();
        REQUIRE(t1 >= t2);
        REQUIRE(item == 1);
    }

    SECTION("when client push many items all waiting threads are notified")
    {
        const int size = 3;

        vector<int> items(size);
        vector<thread> threads(size);

        for (int i = 0; i < size; ++i)
        {
            threads[i] = thread{[&tsq, i, &items] { tsq.pop(items[i]); }};
        }

        tsq.push({1, 2, 3});

        for (auto& thd : threads)
            thd.join();

        REQUIRE(none_of(items.begin(), items.end(), [](int x) { return x == 0; }));
    }
}
