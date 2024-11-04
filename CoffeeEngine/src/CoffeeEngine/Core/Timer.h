#pragma once

#include "CoffeeEngine/Core/Assert.h"
#include "Stopwatch.h"

#include "SDL3/SDL_timer.h"
#include <functional>

namespace Coffee
{
    /**
     * @defgroup core Core
     * @brief Core components of the CoffeeEngine.
     * @{
     */

    class Timer
    {
    public:

        using TimerCallback = std::function<void()>;

        Timer() {}

        Timer(double waitTime, bool autoStart, bool oneShot, TimerCallback callback);

        void Start(double waitTime);
        void Stop();

        void setWaitTime(double waitTime);
        double getWaitTime() const { return m_WaitTime; }

        void setOneShot(bool oneShot) { m_OneShot = oneShot; }
        bool isOneShot() const { return m_OneShot; }

        void setAutoStart(bool autoStart) { m_AutoStart = autoStart; }
        bool isAutoStart() const { return m_AutoStart; }

        void setPaused(bool paused);
        bool isPaused() const { return m_Paused; }

        void isStopped();

        double GetTimeLeft() const { return m_WaitTime - m_Stopwatch.GetElapsedTime(); }

        void SetCallback(TimerCallback callback) { m_Callback = callback; }
        TimerCallback GetCallback() const { return m_Callback; }

    private:
        double m_WaitTime = 1.0f;
        bool m_OneShot = false;
        bool m_AutoStart = false;
        bool m_Paused = false;

        Stopwatch m_Stopwatch;

        TimerCallback m_Callback;
        SDL_TimerID m_TimerID;
    };

    /** @} */
} // namespace Coffee