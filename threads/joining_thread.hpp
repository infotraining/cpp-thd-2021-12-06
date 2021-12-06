#ifndef JOINING_THREAD_HPP
#define JOINING_THREAD_HPP

#include <thread>
#include <iostream>
#include <type_traits>

template <typename T1, typename T2>
constexpr bool is_similar_v = std::is_same<std::decay_t<T1>, std::decay_t<T2>>::value;

class JoiningThread
{
public:
    JoiningThread() = default;

    template<class Function, typename ...Args, typename = std::enable_if_t<!is_similar_v<Function, JoiningThread>>>
    JoiningThread(Function&& fn, Args&&... args)
        : m_thr(std::forward<Function>(fn), std::forward<Args>(args)...)
    {
    }

    JoiningThread(const JoiningThread&) = delete;
    JoiningThread& operator=(const JoiningThread&) = delete;
    JoiningThread(JoiningThread&&) = default;
    JoiningThread& operator=(JoiningThread&&) = default;

    void join()
    {
        m_thr.join();
    }

    void detach()
    {
        m_thr.detach();
    }

    
    std::thread::native_handle_type native_handle()
    {
        return m_thr.native_handle();
    }

    bool joinable() const 
    {
        return m_thr.joinable();
    }

    ~JoiningThread() 
    {
        if (m_thr.joinable())
            m_thr.join();
    }

private:
    std::thread m_thr;
};

#endif