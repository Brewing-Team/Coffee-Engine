//
// Created by mdoradom on 20/09/24.
//

#include "OutputPanel.h"

#include <imgui.h>

namespace Tea {

    void OutputPanel::OnImGuiRender()
    {
        ImGui::Begin("Output");
        ImGui::Text("Hello from Output Panel");
        ImGui::End();
    }

} // Tea