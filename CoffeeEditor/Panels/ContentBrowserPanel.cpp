#include "ContentBrowserPanel.h"
#include "CoffeeEngine/IO/ResourceRegistry.h"
#include "CoffeeEngine/Project/Project.h"
#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Scene/Scene.h"

#include <imgui.h>
#include <filesystem>

namespace Coffee {

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

        if(!Project::GetActive()->GetProjectDirectory().empty())
        {
            if(m_CurrentDirectory != Project::GetActive()->GetProjectDirectory())
            {
                m_CurrentDirectory = Project::GetActive()->GetProjectDirectory();
            }

            DisplayDirectoryContents(m_CurrentDirectory, 0);
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::TextWrapped("No project loaded, create or open a project to see its contents");
            ImGui::PopStyleColor();
        }

        ImGui::End();
    }

    void ContentBrowserPanel::DisplayDirectoryContents(const std::filesystem::path& directory, int depth)
    {
        for (auto& directoryEntry : std::filesystem::directory_iterator(directory))
        {
            const auto& path = directoryEntry.path();
            auto relativePath = std::filesystem::relative(path, m_CurrentDirectory);
            std::string filenameString = relativePath.filename().string();

            ImGuiTreeNodeFlags flags = ((m_SelectedDirectory == path) ? ImGuiTreeNodeFlags_Selected : 0) |
                            (directoryEntry.is_directory() ? 0 : ImGuiTreeNodeFlags_Leaf) |
                            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;

            if (ImGui::TreeNodeEx(filenameString.c_str(), flags))
            {
                if(ImGui::BeginDragDropSource())
                {
                    std::string pathString = path.string();
                    ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", pathString.c_str(), pathString.size() + 1);
                    ImGui::Text("%s", filenameString.c_str());
                    ImGui::EndDragDropSource();
                }

                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                {
                    m_SelectedDirectory = path;
                }
                if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
                {
                    if(relativePath.extension() == ".glb" || relativePath.extension() == ".gltf" || relativePath.extension() == ".obj")
                    {
                        AddModelToTheSceneTree(m_Context.get(), ResourceRegistry::Get<Model>(path.filename().string()));
                    }
                }

                if(directoryEntry.is_directory())
                    DisplayDirectoryContents(path, depth + 1);

                ImGui::TreePop();
            }
        }
    };

}
