#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
    void start();
    /* stops timer and returns millisconds since start, in microsecond resolution */
    float stop();

private:
    bool mRunning = false;
    std::chrono::time_point<std::chrono::steady_clock> mStartTime{};
};

#endif // TIMER_H
