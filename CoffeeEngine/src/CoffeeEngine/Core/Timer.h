#pragma once

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

        //Timer(double waitTime, bool autoStart = false, bool oneShot = false, TimerCallback callback = nullptr);

        void Start(uint32_t waitTime);
        void Stop();

        void setWaitTime(uint32_t waitTime) { m_WaitTime = waitTime; }
        uint32_t getWaitTime() const { return m_WaitTime; }

        void setOneShot(bool oneShot) { m_OneShot = oneShot; }
        bool isOneShot() const { return m_OneShot; }

        void setAutoStart(bool autoStart) { m_AutoStart = autoStart; }
        bool isAutoStart() const { return m_AutoStart; }

        void setPaused(bool paused) { m_Paused = paused; }
        void isPaused() const { return m_Paused; }

        void isStopped();

        void GetTimeLeft() const;

        void SetCallback(TimerCallback callback) { m_Callback = callback; }

    private:
        uint32_t m_WaitTime = 1.0f;
        bool m_OneShot = false;
        bool m_AutoStart = false;
        bool m_Paused = false;

        double m_TimeLeft = 0.0f;

        TimerCallback m_Callback;
        SDL_TimerID m_TimerID;
    };

    /** @} */
} // namespace Coffee