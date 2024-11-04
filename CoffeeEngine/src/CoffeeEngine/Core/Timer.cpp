#include "Timer.h"
#include "CoffeeEngine/Core/Assert.h"
#include "CoffeeEngine/Core/Log.h"
#include "SDL3/SDL_timer.h"

namespace Coffee
{
    SDL_TimerCallback timerCallback = [](void* param, uint32_t interval, uint32_t elapsed) -> uint32_t
    {
        Timer* timer = static_cast<Timer*>(param);

        if(timer->GetCallback())
        {
            timer->GetCallback()();
        }

        if(timer->isOneShot())
        {
            timer->Stop();
            return 0;
        }

        if(timer->isAutoStart())
        {
            timer->Start(timer->getWaitTime());
        }
        return 0;
    };

    Timer::Timer(double waitTime, bool autoStart, bool oneShot, TimerCallback callback)
            : m_WaitTime(waitTime), m_AutoStart(autoStart), m_OneShot(oneShot), m_Callback(callback)
    {
        COFFEE_ASSERT(m_WaitTime > 0, "Time should be greater than zero!");

        if(m_AutoStart) Start(m_WaitTime);
    }

    void Timer::Start(double waitTime)
    {
        m_WaitTime = waitTime;

        m_Stopwatch.Reset();
        m_Stopwatch.Start();

        m_TimerID = SDL_AddTimer(static_cast<Uint32>(m_WaitTime * 1000), timerCallback, this);
        if(m_TimerID == 0)
        {
            COFFEE_ERROR("Timer could not be created: {0}", SDL_GetError());
        }
    }

    void Timer::Stop()
    {
        m_Stopwatch.Stop();

        if(SDL_RemoveTimer(m_TimerID) == false)
        {
            COFFEE_ERROR("Timer could not be removed: {0}", SDL_GetError());
        }

    }

    void Timer::setPaused(bool paused)
    {
        if(paused)
        {
            Stop();
        }
        else
        {
            Start(GetTimeLeft());
        }
    }

    void Timer::setWaitTime(double waitTime)
    {
        COFFEE_ASSERT(waitTime > 0, "Time should be greater than zero!");
        m_WaitTime = waitTime;
    }

} // namespace Coffee