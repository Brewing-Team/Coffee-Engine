#include "SceneTreePanel.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Scene/Components.h"
#include "TeaEngine/Scene/Entity.h"
#include "TeaEngine/Core/Log.h"
#include "TeaEngine/Scene/SceneTree.h"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include <cstdint>
#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <string>


namespace Tea {

    SceneTreePanel::SceneTreePanel(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void SceneTreePanel::SetContext(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void SceneTreePanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Tree");

        //Button for adding entities to the scene tree
        if(ImGui::Button("+", {24,24}))
        {
            m_Context->CreateEntity();
        }
        ImGui::SameLine();

        char searchBuffer[1024];
        ImGui::InputTextWithHint("##searchbar", "Search by name:",searchBuffer, 1024);

        ImGui::BeginChild("entity tree", {0,0}, ImGuiChildFlags_Border);
        
        auto view = m_Context->m_Registry.view<entt::entity>();
        for(auto entityID: view)
        {
            Entity entity{ entityID, m_Context.get()};
            auto& hierarchyComponent = entity.GetComponent<HierarchyComponent>();
            
            if(hierarchyComponent.m_Parent == entt::null)
            {
                DrawEntityNode(entity);
            }
        }

        ImGui::EndChild();

        if(ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            m_SelectionContext = {};
        }

        ImGui::End();

        ImGui::Begin("Inspector");
        if(m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);
        }

        ImGui::End();
    }

    void SceneTreePanel::DrawEntityNode(Entity entity)
    {
        auto& entityNameTag = entity.GetComponent<TagComponent>().Tag;

        auto& hierarchyComponent = entity.GetComponent<HierarchyComponent>();

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) |
                                   ((hierarchyComponent.m_First == entt::null) ? ImGuiTreeNodeFlags_Leaf : 0) |
                                   ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;
        
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, entityNameTag.c_str());

        if(ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        //Code of Double clicking the item for changing the name (WIP)

        ImVec2 itemSize = ImGui::GetItemRectSize();

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            ImVec2 popupPos = ImGui::GetItemRectMin();
            float indent = ImGui::GetStyle().IndentSpacing;
            ImGui::SetNextWindowPos({popupPos.x + indent, popupPos.y});
            ImGui::OpenPopup("EntityPopup");
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        if (ImGui::BeginPopup("EntityPopup"/*, ImGuiWindowFlags_NoBackground*/)) 
        {
            auto buff = entity.GetComponent<TagComponent>().Tag.c_str();
            ImGui::SetNextItemWidth(itemSize.x - ImGui::GetStyle().IndentSpacing);
            ImGui::InputText("##entity-name", (char*)buff, 128);
            ImGui::EndPopup();
        }

        ImGui::PopStyleVar();

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            ImGui::SetDragDropPayload("ENTITY_NODE", &entity, sizeof(Entity)); // Use the entity ID or a pointer as payload
            ImGui::Text("%s", entityNameTag.c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_NODE"))
            {
                // Assuming payload is an Entity, but you need to cast and check appropriately
                Entity payloadEntity = *(const Entity*)payload->Data;
                // Process the drop, e.g., reparenting the entity in the hierarchy
                // This is where you would update the ECS or scene graph
                HierarchyComponent::Reparent(m_Context->m_Registry, (entt::entity)payloadEntity, entity); //I think is not necessary do the casting, it does it automatically;
            }
            ImGui::EndDragDropTarget();
        }

        if(opened)
        {
            if(hierarchyComponent.m_First != entt::null)
            {
                // Recursively draw all children
                Entity childEntity{ hierarchyComponent.m_First, m_Context.get()};
                while((entt::entity)childEntity != entt::null)
                {
                    DrawEntityNode(childEntity);
                    auto& childHierarchyComponent = childEntity.GetComponent<HierarchyComponent>();
                    childEntity = Entity{ childHierarchyComponent.m_Next, m_Context.get() };
                }
            }
            ImGui::TreePop();
        }
    }

    void SceneTreePanel::DrawComponents(Entity entity)
    {
        if(entity.HasComponent<TagComponent>())
        {
            auto& entityNameTag = entity.GetComponent<TagComponent>().Tag;

            ImGui::Text("Tag");
            ImGui::SameLine();

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, entityNameTag.c_str());
            
            if(ImGui::InputText("##", buffer, sizeof(buffer)))
            {
                entityNameTag = std::string(buffer);
            }

            ImGui::Separator();
        }

        if(entity.HasComponent<TransformComponent>())
        {
            auto& transformComponent = entity.GetComponent<TransformComponent>();

            if(ImGui::CollapsingHeader("Transform"))
            {   
                ImGui::Text("Position");
                ImGui::DragFloat3("##Position", glm::value_ptr(transformComponent.Position), 0.1f);
                
                ImGui::Text("Rotation");
                ImGui::DragFloat3("##Rotation", glm::value_ptr(transformComponent.Rotation),  0.1f);

                ImGui::Text("Scale");
                ImGui::DragFloat3("##Scale", glm::value_ptr(transformComponent.Scale),  0.1f);
            }
        }

        if(entity.HasComponent<ModelComponent>())
        {
            auto& modelComponent = entity.GetComponent<ModelComponent>();
            if(ImGui::CollapsingHeader("Model"))
            {   
                auto& meshesArray = modelComponent.model->GetMeshes();
                ImGui::SeparatorText("List of Meshes");

                ImGui::Text("Meshes");
                ImGui::SameLine();
                static bool show;
                if(ImGui::Button(("Vector (size " + std::to_string(meshesArray.size()) + ")").c_str()))
                {
                    show = !show;
                }

                if(show)
                {
                    ImGui::BeginListBox("##Test");
                    for (const auto& mesh : meshesArray)
                    {
                        ImGui::Text("Mesh");
                    }
                    ImGui::EndListBox();
                }
            }
        }

        if(entity.HasComponent<MeshComponent>())
        {
            auto& meshComponent = entity.GetComponent<MeshComponent>();
            if(ImGui::CollapsingHeader("Mesh"))
            {
            }
        }

        if(entity.HasComponent<MaterialComponent>())
        {
            auto& materialComponent = entity.GetComponent<MaterialComponent>();
            if(ImGui::CollapsingHeader("Material"))
            {
            }
        }

        ImGui::Separator();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        float buttonWidth = 200.0f;
        float buttonHeight = 32.0f;
        float availableWidth = ImGui::GetContentRegionAvail().x;
        float cursorPosX = (availableWidth - buttonWidth) * 0.5f;
        ImGui::SetCursorPosX(cursorPosX);

        if(ImGui::Button("Add Component", {buttonWidth, buttonHeight}))
        {
            ImGui::OpenPopup("Add Component...");
        }

        if(ImGui::BeginPopupModal("Add Component..."))
        {
            static char buffer[256] = "";
            ImGui::InputTextWithHint("##Search Component", "Search Component:",buffer, 256);

            std::string items[] = { "Tag Component", "Transform Component", "Mesh Component", "Material Component" };
            static int item_current = 1;

            if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y - 200)))
            {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++)
                {
                    const bool is_selected = (item_current == n);
                    if (ImGui::Selectable(items[n].c_str(), is_selected))
                        item_current = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }
            
            ImGui::Text("Description");
            ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras vel odio lectus. Integer scelerisque lacus a elit consequat, at imperdiet felis feugiat. Nunc rhoncus nisi lacinia elit ornare, eu semper risus consectetur.");
            
            ImGui::Button("Cancel");
            ImGui::SameLine();
            if(ImGui::Button("Add Component"))
            {
                if(items[item_current] == "Tag Component")
                {
                    entity.AddComponent<TagComponent>();
                    ImGui::CloseCurrentPopup();
                }
                else if(items[item_current] == "Transform Component")
                {
                    entity.AddComponent<TransformComponent>();
                    ImGui::CloseCurrentPopup();
                }
                else if(items[item_current] == "Mesh Component")
                {
                    entity.AddComponent<MeshComponent>();
                    ImGui::CloseCurrentPopup();
                }
                else if(items[item_current] == "Material Component")
                {
                    entity.AddComponent<MaterialComponent>();
                    ImGui::CloseCurrentPopup();
                }
                else
                {
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }
    }

}
