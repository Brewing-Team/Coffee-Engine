#pragma once

#include <SDL3/SDL.h>
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

        Timer(double waitTime, bool oneShot = false, bool autoStart = false, TimerCallback callback = nullptr);

        void Start();
        void Stop();
        void TogglePause();
        void Update();

        bool IsProcessing() const;
        bool IsPaused() const;
        double GetTimeLeft() const;

    private:
        Uint32 m_StartTicks;
        Uint32 m_PausedTicks;
        double m_WaitTime;
        bool m_OneShot;
        bool m_AutoStart;
        bool m_Processing;
        bool m_Paused;
        TimerCallback m_Callback;

    };

    /** @} */
} // namespace Coffee