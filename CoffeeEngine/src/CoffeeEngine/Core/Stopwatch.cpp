#include "Stopwatch.h"

#include <SDL3/SDL_timer.h>

namespace Coffee {

    uint64_t Stopwatch::s_Frequency = SDL_GetPerformanceFrequency();

    Stopwatch::Stopwatch()
        : m_StartTicks(0), m_ElapsedTicks(0), m_StartPerfCounter(0), m_ElapsedPerfCounter(0), m_Running(false) {}

    void Stopwatch::Start() {
        if (!m_Running) {
            m_Running = true;
            m_StartTicks = SDL_GetTicks() - m_ElapsedTicks;
            m_StartPerfCounter = SDL_GetPerformanceCounter() - m_ElapsedPerfCounter;
        }
    }

    void Stopwatch::Stop() {
        if (m_Running) {
            m_Running = false;
            m_ElapsedTicks = SDL_GetTicks() - m_StartTicks;
            m_ElapsedPerfCounter = SDL_GetPerformanceCounter() - m_StartPerfCounter;
        }
    }

    void Stopwatch::Reset()
    {
        m_Running = false;
        m_StartTicks = 0;
        m_ElapsedTicks = 0;
        m_StartPerfCounter = 0;
        m_ElapsedPerfCounter = 0;
    }

    float Stopwatch::GetElapsedTime() const {
        if (m_Running) {
            return (SDL_GetTicks() - m_StartTicks) / 1000.0;
        } else {
            return m_ElapsedTicks / 1000.0;
        }
    }

    double Stopwatch::GetPreciseElapsedTime() const {
        if (m_Running) {
            return static_cast<double>(SDL_GetPerformanceCounter() - m_StartPerfCounter) / SDL_GetPerformanceFrequency();
        } else {
            return static_cast<double>(m_ElapsedPerfCounter) / SDL_GetPerformanceFrequency();
        }
    }

} // namespace Coffee