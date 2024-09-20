#include "OutputPanel.h"

#include "TeaEngine/Core/Application.h"

#include <imgui.h>

namespace Tea {

    void OutputPanel::OnImGuiRender()
    {
        ImGui::Begin("Output");
        const auto& logBuffer = Tea::Log::GetLogBuffer();
        for (const auto& log : logBuffer)
        {
            ImGui::TextUnformatted(log.c_str());
        }
        ImGui::End();
    }

} // Tea