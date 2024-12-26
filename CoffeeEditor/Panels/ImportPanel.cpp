#include "ImportPanel.h"

#include "imgui.h"

namespace Coffee {

    ImportPanel::ImportPanel(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void ImportPanel::SetContext(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void ImportPanel::OnImGuiRender()
    {
        if (!m_Visible) return;

        ImGui::Begin("Import");

        // If there is no resource selected, display a message in the center of the panel:
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        ImGui::TextWrapped("No resource selected!");
        ImGui::PopStyleColor();

        // Then another message below the first one with a different color and text:
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::TextWrapped("Select a resource from the content browser to adjust its properties");
        ImGui::PopStyleColor();

        ImGui::End();
    }

}