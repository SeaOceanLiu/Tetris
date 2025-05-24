#ifndef TimerH
#define TimerH
#include <functional> // std::function
#include <SDL3/SDL.h>

typedef std::function<void(void *userdata)> TimerCallback;
class Timer
{
private:
    SDL_TimerID m_timerId;
    Uint32 m_interval;
    TimerCallback m_onTimerCallback;
    void *m_userdata;
    bool m_isRunning;
    bool m_isLooping;
    Uint64 m_startTicks;
    Uint64 m_ticksElapsed;
public:
    Timer();
    ~Timer();

    bool start(TimerCallback onTimercallback, void *userdata, Uint32 millisecondsInterval, bool isLooping=false);
    void stop();

    static Uint32 timerCallback(void *userdata, SDL_TimerID timerID, Uint32 millisecondsInterval);
};
#endif