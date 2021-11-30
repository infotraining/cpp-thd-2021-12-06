#include <atomic>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
    const long N = 100'000'000;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    //////////////////////////////////////////////////////////////////////////////
    // single thread

    cout << "Pi calculation started!" << endl;
    const auto start = chrono::high_resolution_clock::now();
        
    long hits = 0;

    for (long n = 0; n < N; ++n)
    {
        double x = rand() / static_cast<double>(RAND_MAX);
        double y = rand() / static_cast<double>(RAND_MAX);
        if (x * x + y * y < 1)
            hits++;
    }

    const double pi = static_cast<double>(hits) / N * 4;

    const auto end = chrono::high_resolution_clock::now();
    const auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "Pi = " << pi << endl;
    cout << "Elapsed = " << elapsed_time << "ms" << endl;

    //////////////////////////////////////////////////////////////////////////////
}
