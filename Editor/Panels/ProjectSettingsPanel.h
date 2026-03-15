#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/EngineContext.h"
#include "Panel.h"
#include "imgui.h"

#include <stdint.h>
#include <array>
#include <string>

namespace Coffee {
    class InputBinding;

    enum class SettingsCategory : uint8_t
    {
        None = 0,
        General,
        InputMap,
        Display,
        Audio,
        Physics,
        Rendering,
        Debug
    };

    class ProjectSettingsPanel : public Panel
    {
    public:
        ProjectSettingsPanel() = default;

        void SetContext(const EngineContext& context) { m_Context = &context; }

        void OnImGuiRender() override;

    private:
        // Rendering methods
        void RenderSearchBar();
        void RenderCategoryTree();
        void RenderSettingsContent();
        
        // Category-specific rendering
        void RenderGeneralSettings();
        void RenderInputMapSettings();
        void RenderDisplaySettings();
        void RenderAudioSettings();
        void RenderPhysicsSettings();
        void RenderRenderingSettings();
        void RenderDebugSettings();
        
        // Input Map helpers
        void RenderInputActionsList();
        void RenderInputActionDetails();
        void RenderAddActionPopup();
        void SetSelectedAction(const std::string& actionName);
        
        // Data members
        SettingsCategory m_CurrentCategory = SettingsCategory::General;
        std::array<char, 256> m_SearchBuffer;
        std::string m_SearchQuery;
        
        // Debug state
        std::array<char, 256> m_ResourceSearchBuffer;
        std::string m_ResourceSearchQuery;
        bool m_ShowImGuiDemoWindow = false;
        
        // Input Map state
        std::string m_SelectedActionName;
        InputBinding* m_SelectedAction = nullptr;
        std::array<char, 256> m_NewActionNameBuffer;
        std::array<char, 256> m_RenameActionBuffer;
        bool m_ShowAddActionPopup = false;
        bool m_WaitingForInput = false;
        int m_RebindingIndex = -1;

        const EngineContext* m_Context = nullptr;
    };

} // Coffee
