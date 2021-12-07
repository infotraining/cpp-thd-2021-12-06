#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

class BankAccount
{
    const int id_;
    double balance_;
    mutable std::shared_mutex mtx_;

public:
    BankAccount(int id, double balance)
        : id_(id)
        , balance_(balance)
    {
    }

    void print() const
    {
        double balance_copy = balance();

        std::cout << "Bank Account #" << id_ << "; Balance = " << balance_copy << std::endl;
    }

    void transfer(BankAccount& to, double amount)
    {
        std::scoped_lock lock(mtx_, to.mtx_);

        balance_ -= amount;
        to.balance_ += amount;
    }

    void withdraw(double amount)
    {
        std::scoped_lock lock(mtx_);

        balance_ -= amount;
    }

    void deposit(double amount)
    {
        std::scoped_lock lock(mtx_);

        balance_ += amount;
    }

    int id() const
    {
        return id_;
    }

    double balance() const
    {
        std::shared_lock lock(mtx_);

        return balance_;
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

std::timed_mutex mtx;

void background_worker(int id, int timeout)
{
    std::cout << "THD#" << id << " is waiting for mutex..." << std::endl;

    std::unique_lock lk(mtx, std::try_to_lock);

    if (!lk.owns_lock())
    {
        do
        {
            std::cout << "THD#" << id << " doesn't own lock..."
                      << " Tries to acquire mutex..." << std::endl;
        } while (!lk.try_lock_for(std::chrono::milliseconds(timeout)));
    }

    std::cout << "Start of THD#" << id << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::cout << "End of THD#" << id << std::endl;
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

    thd1.join();
    thd2.join();

    std::cout << "After all threads are done: ";
    ba1.print();
    ba2.print();

    std::cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";

    {
        std::thread thd1(&background_worker, 1, 500);
        std::thread thd2(&background_worker, 2, 750);

        thd1.join();
        thd2.join();
    }
}
