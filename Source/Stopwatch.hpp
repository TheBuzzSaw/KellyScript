#ifndef StopwatchHpp
#define StopwatchHpp

#include <chrono>

class Stopwatch
{
    std::chrono::high_resolution_clock::time_point _start;
public:
    inline Stopwatch()
    {
        Restart();
    }

    Stopwatch(const Stopwatch&) = default;
    ~Stopwatch() = default;

    Stopwatch& operator=(const Stopwatch&) = default;

    inline void Restart()
    {
        _start = std::chrono::high_resolution_clock::now();
    }

    inline double Seconds()
    {
        std::chrono::duration<double> gap = std::chrono::high_resolution_clock::now() - _start;
        return gap.count();
    }
};

#endif
