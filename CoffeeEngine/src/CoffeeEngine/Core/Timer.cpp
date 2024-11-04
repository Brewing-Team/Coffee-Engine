#include "Timer.h"

namespace Coffee
{
    void Timer::Start(uint32_t waitTime)
    {
        m_WaitTime = waitTime;

        m_TimerID = SDL_AddTimer(m_WaitTime, m_Callback, this);
    }

} // namespace Coffee