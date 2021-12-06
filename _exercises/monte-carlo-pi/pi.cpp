#include <atomic>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <numeric>
#include <random>
#include <thread>

using namespace std;

void monte_carlo(size_t seed, long iterations, long& hits)
{
    mt19937_64 rnd_gen {seed};
    uniform_real_distribution<> distr(-1, 1);

    long local_hits = 0;

    for (long n = 0; n < iterations; ++n)
    {
        double x = distr(rnd_gen);
        double y = distr(rnd_gen);
        if (x * x + y * y < 1)
        {
            local_hits++;
        //    hits++;
        }
    }

    hits += local_hits;
}

int main()
{
    const long N = 100'000'000;
    std::random_device rd;

    {
        cout << "Pi calculation started! - one thread" << endl;
        const auto start = chrono::high_resolution_clock::now();

        long hits{};

        monte_carlo(rd(), N, hits);
        
        const double pi = static_cast<double>(hits) / N * 4;

        const auto end = chrono::high_resolution_clock::now();
        const auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout << "Pi = " << pi << endl;
        cout << "Elapsed = " << elapsed_time << "ms" << endl;
    }

    {
        cout << "Pi calculation started! - multithreading" << endl;
        const auto start = chrono::high_resolution_clock::now();

        const std::size_t no_of_threads = std::max(1u, std::thread::hardware_concurrency());
        cout << "Running " << no_of_threads << " threads" << endl;

        std::vector<std::thread> threads(no_of_threads);
        std::vector<long> results(no_of_threads);

        for (int i = 0; i < threads.size(); ++i)
        {
            threads[i] = std::thread {monte_carlo, rd(), N / no_of_threads, std::ref(results[i])};
        }

        for (auto& t : threads)
        {
            t.join();
        }

        long hits = std::accumulate(results.begin(), results.end(), 0l);

        const double pi = static_cast<double>(hits) / N * 4;

        const auto end = chrono::high_resolution_clock::now();
        const auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout << "Pi = " << pi << endl;
        cout << "Elapsed = " << elapsed_time << "ms" << endl;
    }

    //////////////////////////////////////////////////////////////////////////////
}