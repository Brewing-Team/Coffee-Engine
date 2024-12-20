#include "ContentBrowserPanel.h"
#include "CoffeeEngine/IO/Resource.h"
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
        if (!m_Visible) return;

        ImGui::Begin("Content Browser");

        if(Project::GetActive() and !Project::GetActive()->GetProjectDirectory().empty())
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
                    Ref<Resource> resource = ResourceRegistry::Get<Resource>(path.filename().string());
                    ImGui::SetDragDropPayload("RESOURCE", &resource, sizeof(Ref<Resource>));
                    ImGui::Text("%s", filenameString.c_str());
                    ImGui::EndDragDropSource();
                }

                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                {
                    m_SelectedDirectory = path;
                }
                if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
                {
                    
                }
                //if (ImGui::BeginItemTooltip())
                //{
                //    const Ref<Resource>& resource = ResourceRegistry::Get<Resource>(path.filename().string());
                //    ImGui::Text(path.filename().string().c_str());
                //    ImGui::Text("Size: {0}", std::filesystem::file_size(path));
                //    ImGui::Text("Type: {0}", resource->GetTypeAsString());
                //    ImGui::EndTooltip();
                //}

                if(directoryEntry.is_directory())
                    DisplayDirectoryContents(path, depth + 1);

                ImGui::TreePop();
            }
        }
    };

}
