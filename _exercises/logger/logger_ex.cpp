#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

namespace Before
{
    class Logger
    {
        ofstream fout_;

    public:
        Logger(const string& file_name)
        {
            fout_.open(file_name);
        }

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        ~Logger()
        {
            fout_.close();
        }

        void log(const string& message)
        {
            fout_ << message << endl;
            fout_.flush();
        }
    };
}

using namespace Before;

void run(Logger& logger, int id)
{
    for (int i = 0; i < 1000; ++i)
        logger.log("Log#" + to_string(id) + " - Event#" + to_string(i));
}

int main()
{
    /*
     * TODO: Write a Logger class which is thread-safe
     */

    Logger log("data.log");

    thread thd1(&run, ref(log), 1);
    thread thd2(&run, ref(log), 2);

    thd1.join();
    thd2.join();
}
