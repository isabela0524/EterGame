#include "Timer.h"

void Timer::run()
{
    while (m_running && m_remaining_time > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        --m_remaining_time;
    }
    if (m_remaining_time == 0) {
        m_callback();
    }
}

Timer::Timer(int seconds, std::function<PlayerEnum()> func)
    :m_initial_duration(seconds), m_remaining_time(seconds), m_callback(func), m_running(false)
{
}

Timer::~Timer()
{
    stop();
}

void Timer::start()
{
    std::lock_guard<std::mutex> lock(m_timer_mutex);
    if (m_running)
        return;
    m_running = true;
    m_timer_thread = std::thread(&Timer::run, this);
}

void Timer::stop()
{
    std::lock_guard<std::mutex> lock(m_timer_mutex);
    m_running = false;
    if (m_timer_thread.joinable())
    {
        m_timer_thread.join();
    }
}

bool Timer::hasTimeLeft() const
{
    return m_remaining_time > 0;
}

bool Timer::isTimer() const
{
    return m_initial_duration != -1;
}

void Timer::resetTimer()
{
    std::lock_guard<std::mutex> lock(m_timer_mutex);
    stop();
    m_remaining_time = m_initial_duration;
}

bool Timer::operator==(const Timer& other) const
{
    return m_initial_duration == other.m_initial_duration &&
        m_remaining_time == other.m_remaining_time &&
        m_running == other.m_running;
}