#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <condition_variable>
#include <mutex>
#include <queue>


template <typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue() = default;

    ThreadSafeQueue(const ThreadSafeQueue&) = default;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = default;

    bool empty() const
    {
        std::lock_guard<std::mutex> l(mtx);

        return queue.empty();
    }

    void push(const T& val)
    {
        {
            std::lock_guard<std::mutex> l(mtx);

            queue.push(val);
        }
        cv_q_not_empty.notify_one();
    }

    void push(T&& val)
    {
        {
            std::lock_guard<std::mutex> l(mtx);

            queue.push(std::move(val));
        }
        cv_q_not_empty.notify_one();
    }

    void push(std::initializer_list<T> it)
    {
        {
            std::lock_guard<std::mutex> l(mtx);

            for (const auto& v : it)
                queue.push(v);
        }
        cv_q_not_empty.notify_all(); 
    }

    void pop(T& item)
    {
        std::unique_lock<std::mutex> l(mtx);

        cv_q_not_empty.wait(l, [this] { return !queue.empty(); });

        item = queue.front();
        queue.pop();
    }

    bool try_pop(T& val)
    {
        std::unique_lock<std::mutex> l(mtx, std::try_to_lock);

        if (l.owns_lock() && !queue.empty())
        {
            val = queue.front();
            queue.pop();
            return true;
        }

        return false;
    }

private:
    mutable std::mutex mtx;
    std::condition_variable cv_q_not_empty;
    std::queue<T> queue;
};

#endif // THREAD_SAFE_QUEUE_HPP
