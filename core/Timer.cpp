#include "Timer.h"

Timer::Timer():
    m_timerId(0),
    m_interval(0),
    m_onTimerCallback(nullptr),   // 貌似无法这样初始化std::function
    m_isRunning(false),
    m_startTicks(0),
    m_isLooping(false),
    m_userdata(nullptr),
    m_ticksElapsed(0)
{

}

Timer::~Timer()
{
    stop();
}

// 定时器使用后台线程调用回调函数，所以回调函数中所使用的资源，需要使用线程锁保护
bool Timer::start(TimerCallback onTimercallback, void *userdata, Uint32 millisecondsInterval, bool isLooping)
{
    m_onTimerCallback = onTimercallback;
    m_interval = millisecondsInterval;
    m_userdata = userdata;
    m_isLooping = isLooping;
    m_timerId = SDL_AddTimer(millisecondsInterval, Timer::timerCallback, this);
    if (m_timerId == 0)
    {
        SDL_Log("Failed to start timer: %s", SDL_GetError());
        return false;
    }
    m_startTicks = SDL_GetPerformanceCounter();

    m_isRunning = true;
    return true;
}

Uint32 Timer::timerCallback(void *userdata, SDL_TimerID timerID, Uint32 millisecondsInterval)
{
    Timer *timer = (Timer *)userdata;
    // Todo: 这里可能会在退出时正好触发，需要做保护
    timer->m_onTimerCallback(timer->m_userdata);

    if (timer->m_isLooping) {
        return millisecondsInterval;
    }
    else
    {
        timer->stop();
        return 0;
    }
}

void Timer::stop()
{
    if (m_timerId != 0){
        if(!SDL_RemoveTimer(m_timerId)){
            SDL_Log("Failed to remove timer: %s", SDL_GetError());
        }
        m_timerId = 0;
    }
    m_isRunning = false;
}
