#include "ContentBrowserPanel.h"

#include <imgui.h>

#include <filesystem>

namespace Tea {

    // TODO Change this path to be the assets path of the project
    constexpr const char* s_AssetPath = "assets";

    ContentBrowserPanel::ContentBrowserPanel(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void ContentBrowserPanel::SetContext(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        ImGui::Begin("Content Browser");

        for (auto& p : std::filesystem::directory_iterator(s_AssetPath) )
        {
            ImGui::Text("%s", p.path().string().c_str());
        }

        ImGui::End();
    }

}
