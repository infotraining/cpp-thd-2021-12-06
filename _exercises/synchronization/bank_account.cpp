#include <iostream>
#include <thread>

class BankAccount
{
    const int id_;
    double balance_;

public:
    BankAccount(int id, double balance)
        : id_(id)
        , balance_(balance)
    {
    }

    void print() const
    {
        std::cout << "Bank Account #" << id_ << "; Balance = " << balance_ << std::endl;
    }

    void transfer(BankAccount& to, double amount)
    {
        balance_ -= amount;
        to.balance_ += amount;
    }

    void withdraw(double amount)
    {
        balance_ -= amount;
    }

    void deposit(double amount)
    {
        balance_ += amount;
    }

    int id() const
    {
        return id_;
    }

    double balance() const
    {
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
}
