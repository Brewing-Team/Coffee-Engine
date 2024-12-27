#include "ImportPanel.h"

#include "CoffeeEngine/Core/Application.h"
#include "imgui.h"

#include <src/EditorLayer.h>

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

        if (m_Context)
        {
            if (selectedResource)
            {
                ImGui::Text("Selected Resource: %s", selectedResource->GetName().c_str());
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::TextWrapped("No resource selected!");
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                ImGui::TextWrapped("Select a resource from the content browser to adjust its properties");
                ImGui::PopStyleColor();
            }
        }

        ImGui::End();
    }

}