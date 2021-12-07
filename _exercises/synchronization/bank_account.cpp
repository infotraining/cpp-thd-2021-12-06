#include <iostream>
#include <thread>
#include <mutex>

class BankAccount
{
    const int id_;
    double balance_;
    mutable std::recursive_mutex mtx_balance_;

public:
    BankAccount(int id, double balance)
        : id_(id)
        , balance_(balance)
    {
    }

    void print() const
    {
        double current_balance; 
        {
            std::lock_guard lk{mtx_balance_};
            current_balance = this->balance_;
        }
        std::cout << "Bank Account #" << id_ << "; Balance = " << current_balance << std::endl;

        //std::cout << "Bank Account #" << id_ << "; Balance = " << balance() << std::endl;
    }

    void transfer(BankAccount& to, double amount)
    {
        // std::lock(mtx_balance_, to.mtx_balance_);
        // std::lock_guard lock1{mtx_balance_, std::adopt_lock};
        // std::lock_guard lock2{to.mtx_balance_, std::adopt_lock};

        // std::unique_lock<std::mutex> lk_from(mtx_balance_, std::defer_lock);
        // std::unique_lock<std::mutex> lk_to(to.mtx_balance_, std::defer_lock);
        // std::lock(lk_from, lk_to);

        // balance_ -= amount;
        // to.balance_ += amount;
        
        std::scoped_lock lks{mtx_balance_, to.mtx_balance_};
        withdraw(amount);
        to.deposit(amount);
    }

    void withdraw(double amount)
    {
        std::lock_guard lk{mtx_balance_};
        balance_ -= amount;
    }

    void deposit(double amount)
    {
        std::lock_guard lk{mtx_balance_};
        balance_ += amount;
    }

    int id() const
    {
        return id_;
    }

    double balance() const
    {
        std::lock_guard lk{mtx_balance_};
        return balance_;
    }

    void lock() 
    {
        mtx_balance_.lock();
    }

    void unlock()
    {
        mtx_balance_.unlock();
    }

    bool try_lock()
    {
        return mtx_balance_.try_lock();
    }

    auto with_lock() const
    {
        return std::unique_lock{mtx_balance_};
    }
};

void make_withdraws(BankAccount& ba, int no_of_operations)
{
    for (int i = 0; i < no_of_operations; ++i)
        ba.withdraw(1.0);
}

void make_deposits(BankAccount& ba, int no_of_operations)
{
    for (int i = 0; i < no_of_operations; ++i)
        ba.deposit(1.0);
}

void make_transfers(BankAccount& from, BankAccount& to, int no_of_operations)
{
    for (int i = 0; i < no_of_operations; ++i)
        from.transfer(to, 1.0);
}

void use(const BankAccount& ba)
{
    auto lk = ba.with_lock();
    std::cout << ba.balance() << "\n";
    //...
    std::cout << ba.balance() << "\n";
}

int main()
{
    const int NO_OF_ITERS = 10'000'000;

    BankAccount ba1(1, 10'000);
    BankAccount ba2(2, 10'000);

    std::cout << "Before threads are started: ";
    ba1.print();
    ba2.print();

    std::thread thd1(&make_withdraws, std::ref(ba1), NO_OF_ITERS);
    std::thread thd2(&make_deposits, std::ref(ba1), NO_OF_ITERS);
    std::thread thd3(&make_transfers, std::ref(ba1), std::ref(ba2), NO_OF_ITERS);
    std::thread thd4(&make_transfers, std::ref(ba2), std::ref(ba1), NO_OF_ITERS);

    {  // transaction
        int x = 10; 
        std::scoped_lock trans_lock{ba1, ba2}; // lock - acquire
        ba1.deposit(1'000);
        ba1.transfer(ba2, 500);
        ba1.transfer(ba2, 100);
    } // unlock - release

    {
        auto lk = ba1.with_lock(); // starts transaction
        ba1.deposit(100);
        ba1.deposit(100);
        ba1.withdraw(50);
    }

    thd1.join();
    thd2.join();
    thd3.join();
    thd4.join();

    std::cout << "After all threads are done: ";
    ba1.print();
    ba2.print();
}
