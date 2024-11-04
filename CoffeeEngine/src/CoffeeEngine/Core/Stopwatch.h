#pragma once

#include <SDL3/SDL.h>

namespace Coffee {

    /**
     * @defgroup core Core
     * @brief Core components of the CoffeeEngine.
     * @{
     */

    /**
     * @class Stopwatch
     * @brief A class to measure elapsed time.
     *
     * The Stopwatch class provides functionality to start, stop, reset, and retrieve
     * the elapsed time. It supports both regular and high-precision timing.
     */
    class Stopwatch {
    public:
        /**
         * @brief Constructs a new Stopwatch object.
         */
        Stopwatch();

        /**
         * @brief Starts the stopwatch.
         *
         * If the stopwatch is already running, this method has no effect.
         */
        void Start();

        /**
         * @brief Stops the stopwatch.
         *
         * If the stopwatch is not running, this method has no effect.
         */
        void Stop();

        /**
         * @brief Resets the stopwatch.
         *
         * This method stops the stopwatch and resets the elapsed time to zero.
         */
        void Reset();

        /**
         * @brief Gets the elapsed time in seconds.
         *
         * @return The elapsed time in seconds.
         */
        Uint32 GetElapsedTime() const;

        /**
         * @brief Gets the precise elapsed time in seconds.
         *
         * This method uses SDL_GetPerformanceCounter for higher precision.
         *
         * @return The precise elapsed time in seconds.
         */
        Uint64 GetPreciseElapsedTime() const;

    private:
        Uint32 m_StartTicks; ///< The start time in milliseconds.
        Uint32 m_ElapsedTicks; ///< The elapsed time in milliseconds.
        Uint64 m_StartPerfCounter; ///< The start time in performance counter ticks.
        Uint64 m_ElapsedPerfCounter; ///< The elapsed time in performance counter ticks.
        bool m_Running; ///< Indicates whether the stopwatch is currently running.
        static Uint64 s_Frequency; ///< The frequency of the performance counter.
    };

    /** @} */

} // namespace Coffee