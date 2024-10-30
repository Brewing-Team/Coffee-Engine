#pragma once

#include "Panels/Panel.h"

namespace Coffee {
    class MonitorPanel : public Panel
    {
    public:
        MonitorPanel() = default;
        void OnImGuiRender() override;
    private:
        bool m_ShowFPS = true;
        bool m_ShowFrameTime = true;
        bool m_MemoryUsage = true;
    };
}