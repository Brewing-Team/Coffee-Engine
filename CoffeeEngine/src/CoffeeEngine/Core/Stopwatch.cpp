#include "Stopwatch.h"

namespace Coffee {

    Uint64 Stopwatch::s_Frequency = SDL_GetPerformanceFrequency();

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

    Uint32 Stopwatch::GetElapsedTime() const {
        if (m_Running) {
            return SDL_GetTicks() - m_StartTicks;
        } else {
            return m_ElapsedTicks;
        }
    }

    Uint64 Stopwatch::GetPreciseElapsedTime() const {
        if (m_Running) {
            return SDL_GetPerformanceCounter() - m_StartPerfCounter;
        } else {
            return m_ElapsedPerfCounter;
        }
    }

} // namespace Coffee