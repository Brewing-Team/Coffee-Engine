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

        std::function<void(const std::filesystem::path&, int)> displayDirectoryContents;
        displayDirectoryContents = [&](const std::filesystem::path& directory, int depth)
        {
            for (auto& directoryEntry : std::filesystem::directory_iterator(directory))
            {
                const auto& path = directoryEntry.path();
                auto relativePath = std::filesystem::relative(path, s_AssetPath);
                std::string filenameString = relativePath.filename().string();

                if (depth > 0) // Only indent if not at the root level
                {
                    ImGui::Indent(10.0f); // Indent based on depth
                }

                if (directoryEntry.is_directory())
                {
                    if (ImGui::CollapsingHeader(filenameString.c_str()))
                    {
                        displayDirectoryContents(path, depth + 1);
                    }
                }
                else
                {
                    ImGui::Text(filenameString.c_str());
                }

                if (depth > 0) // Only unindent if not at the root level
                {
                    ImGui::Unindent(10.0f); // Unindent after processing
                }
            }
        };

        displayDirectoryContents(m_CurrentDirectory, 0);

        ImGui::End();
    }

}
