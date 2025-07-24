/**
 * @defgroup core Core
 * @brief Core components of the CoffeeEngine.
 * @{
 */

#pragma once

#include "Stopwatch.h"
#include <functional>

namespace Coffee
{
    /**
     * @class Timer
     * @brief A class that represents a timer with various functionalities.
     */
    class Timer
    {
    public:
        using TimerCallback = std::function<void()>;

        /**
         * @brief Default constructor.
         */
        Timer() {}

        /**
         * @brief Parameterized constructor.
         * @param waitTime The time to wait before the timer triggers.
         * @param autoStart Whether the timer should start automatically.
         * @param oneShot Whether the timer should trigger only once.
         * @param callback The callback function to be called when the timer triggers.
         */
        Timer(double waitTime, bool autoStart, bool oneShot, TimerCallback callback);

        /**
         * @brief Starts the timer with a specified wait time.
         * @param waitTime The time to wait before the timer triggers.
         */
        void Start(double waitTime);

        /**
         * @brief Stops the timer.
         */
        void Stop();

        /**
         * @brief Sets the wait time for the timer.
         * @param waitTime The time to wait before the timer triggers.
         */
        void setWaitTime(double waitTime);

        /**
         * @brief Gets the wait time of the timer.
         * @return The time to wait before the timer triggers.
         */
        double getWaitTime() const { return m_WaitTime; }

        /**
         * @brief Sets whether the timer should trigger only once.
         * @param oneShot Whether the timer should trigger only once.
         */
        void setOneShot(bool oneShot) { m_OneShot = oneShot; }

        /**
         * @brief Checks if the timer is set to trigger only once.
         * @return True if the timer is set to trigger only once, false otherwise.
         */
        bool isOneShot() const { return m_OneShot; }

        /**
         * @brief Sets whether the timer should start automatically.
         * @param autoStart Whether the timer should start automatically.
         */
        void setAutoStart(bool autoStart) { m_AutoStart = autoStart; }

        /**
         * @brief Checks if the timer is set to start automatically.
         * @return True if the timer is set to start automatically, false otherwise.
         */
        bool isAutoStart() const { return m_AutoStart; }

        /**
         * @brief Sets whether the timer is paused.
         * @param paused Whether the timer is paused.
         */
        void setPaused(bool paused);

        /**
         * @brief Checks if the timer is paused.
         * @return True if the timer is paused, false otherwise.
         */
        bool isPaused() const { return m_Paused; }

        /**
         * @brief Checks if the timer is stopped.
         */
        void isStopped();

        /**
         * @brief Gets the remaining time before the timer triggers.
         * @return The remaining time before the timer triggers.
         */
        double GetTimeLeft() const { return m_WaitTime - m_Stopwatch.GetElapsedTime(); }

        /**
         * @brief Sets the callback function to be called when the timer triggers.
         * @param callback The callback function.
         */
        void SetCallback(TimerCallback callback) { m_Callback = callback; }

        /**
         * @brief Gets the callback function to be called when the timer triggers.
         * @return The callback function.
         */
        TimerCallback GetCallback() const { return m_Callback; }

    private:
        double m_WaitTime = 1.0f; ///< The time to wait before the timer triggers.
        bool m_OneShot = false; ///< Whether the timer should trigger only once.
        bool m_AutoStart = false; ///< Whether the timer should start automatically.
        bool m_Paused = false; ///< Whether the timer is paused.

        Stopwatch m_Stopwatch; ///< Stopwatch to measure elapsed time.

        TimerCallback m_Callback; ///< The callback function to be called when the timer triggers.
        int m_TimerID; ///< The SDL timer ID.
    };

    /** @} */
} // namespace Coffee