#pragma once

#include "Panel.h"

namespace Coffee {

    namespace PanelDisplayEnum
    {
        enum : uint8_t{
            None = 0,
            General = BIT(1),
            Input = BIT(2)
        };
    }

    class ProjectSettingsPanel : public Panel
    {
    public:
        ProjectSettingsPanel() = default;

        void OnImGuiRender() override;

    private:

        void BeginHorizontalChild(const char* label, ImGuiWindowFlags flags = ImGuiWindowFlags_None);
        void RenderInputSettings(ImGuiWindowFlags flags);
        void RenderGeneralSettings(ImGuiWindowFlags flags);

        uint8_t m_VisiblePanels = 0;
    };

} // Coffee
