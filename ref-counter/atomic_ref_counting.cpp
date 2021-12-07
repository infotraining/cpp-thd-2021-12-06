#include <atomic>
#include <boost/intrusive_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <iostream>
#include <thread>

// definicja funkcji intrusive_ptr_add_ref i intrusive_ptr_release
template <typename T>
void intrusive_ptr_add_ref(T* t)
{
    t->add_ref();
}

template <typename T>
void intrusive_ptr_release(T* t)
{
    t->release();
}

namespace ThreadUnsafe
{
    // klasa licznika
    template <typename T>
    class RefCounted : boost::noncopyable
    {
        int ref_count_{0};

    public:
        RefCounted() = default;

        void add_ref()
        {
            ++ref_count_;
        }

        void release()
        {
            if (--ref_count_ == 0)
            {
                delete static_cast<T*>(this);
            }
        }
    };
}

namespace ThreadSafe
{
    // klasa licznika
    template <typename T>
    class RefCounted : boost::noncopyable
    {
        std::atomic<int> ref_count_{0};

    public:
        RefCounted() = default;

        void add_ref()
        {
            ref_count_.fetch_add(1, std::memory_order_relaxed);
        }

        void release()
        {
            if (ref_count_.fetch_sub(1, std::memory_order_acq_rel) == 1)
            {
                delete static_cast<T*>(this);
            }
        }
    };
}

// zarzadzana klasa
class Gadget : public ThreadSafe::RefCounted<Gadget>
{
public:
    Gadget()
    {
        std::cout << "Gadget::Gadget()\n";
    }

    Gadget(const Gadget&) = delete;
    Gadget& operator=(const Gadget&) = delete;

    ~Gadget()
    {
        std::cout << "Gadget::~Gadget()\n";
    }
};

int main()
{
    using namespace std;

    thread thd1, thd2;
    {
        boost::intrusive_ptr<Gadget> p1(new Gadget());
        {
            boost::intrusive_ptr<Gadget> p2 = p1;

            thd1 = std::thread([p2] { std::this_thread::sleep_for(1000ms); });
        }

        thd2 = std::thread([p1] { std::this_thread::sleep_for(1000ms); });
    }

    thd1.join();
    thd2.join();
}
