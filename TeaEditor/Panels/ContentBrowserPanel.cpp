#include "ContentBrowserPanel.h"

#include <imgui.h>
#include <filesystem>

namespace Tea {

    // TODO Change this path to be the assets path of the project
    static const std::filesystem::path s_AssetPath = "assets";

    ContentBrowserPanel::ContentBrowserPanel(const Ref<Scene>& scene)
    {
        m_Context = scene;
        m_CurrentDirectory = s_AssetPath;
    }

    void ContentBrowserPanel::SetContext(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        ImGui::Begin("Content Browser");

        if (m_CurrentDirectory != s_AssetPath)
        {
            if (ImGui::Button("<-"))
            {
                m_CurrentDirectory = m_CurrentDirectory.parent_path();
            }
        }

        for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory) )
        {
            const auto& path = directoryEntry.path();
            auto relativePath = std::filesystem::relative(path, s_AssetPath);
            std::string filenameString = relativePath.filename().string();
            
            if (directoryEntry.is_directory())
            {
                if (ImGui::Button(filenameString.c_str()))
                {
                    m_CurrentDirectory /= path.filename();

                    TEA_INFO(m_CurrentDirectory.string());
                }
            }
            else
            {
                ImGui::Text(directoryEntry.path().filename().string().c_str());
            }
        }
        ImGui::End();
    }

}
