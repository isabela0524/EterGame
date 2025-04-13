#pragma once
#include <iostream>
#include <thread>
#include <functional>
#include <atomic>
#include <chrono>
#include <mutex>
#include "PlayerEnum.h"

class Timer
{
private:
    int                             m_initial_duration;
    int                             m_remaining_time;
    std::function<PlayerEnum()>     m_callback;
    std::atomic<bool>               m_running;
    std::thread                     m_timer_thread;
    std::mutex                      m_timer_mutex;

    void                            run();
public:
    explicit Timer(int seconds, std::function<PlayerEnum()> func);
    ~Timer();

    void                            start();
    void                            stop();
    bool                            hasTimeLeft() const;
    bool                            isTimer() const;
    void                            resetTimer();

    bool operator==(const Timer& other) const;
};

