#include "ProjectSettingsPanel.h"

#include "CoffeeEngine/Core/FileDialog.h"
#include "CoffeeEngine/Core/Input.h"
#include "CoffeeEngine/Project/Project.h"
#include "CoffeeEngine/Audio/Audio.h"

#include <imgui.h>
#include <algorithm>

namespace Coffee {

    void ProjectSettingsPanel::SetSelectedAction(const std::string& actionName)
    {
        auto& bindings = Input::GetAllBindings();
        auto it = bindings.find(actionName);
        if (it != bindings.end())
        {
            m_SelectedActionName = actionName;
            m_SelectedAction = &it->second;
            
            // Copy name to rename buffer
            size_t copySize = std::min<size_t>(actionName.size(), 255);
            std::copy_n(actionName.begin(), copySize, m_RenameActionBuffer.begin());
            m_RenameActionBuffer[copySize] = '\0';
        }
    }
    void ProjectSettingsPanel::RenderSearchBar()
    {
        ImGui::SetNextItemWidth(-1.0f);
        if (ImGui::InputTextWithHint("##SearchSettings", "Search settings...", m_SearchBuffer.data(), m_SearchBuffer.size()))
        {
            m_SearchQuery = std::string(m_SearchBuffer.data());
        }
    }

    void ProjectSettingsPanel::RenderCategoryTree()
    {
        ImGui::BeginChild("CategoryTree", ImVec2(200, 0), true);
        
        ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        
        // General category
        ImGuiTreeNodeFlags node_flags = base_flags | (m_CurrentCategory == SettingsCategory::General ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
        bool node_open = ImGui::TreeNodeEx("General", node_flags);
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            m_CurrentCategory = SettingsCategory::General;
        if (node_open)
            ImGui::TreePop();
        
        // Input Map category
        node_flags = base_flags | (m_CurrentCategory == SettingsCategory::InputMap ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
        node_open = ImGui::TreeNodeEx("Input Map", node_flags);
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            m_CurrentCategory = SettingsCategory::InputMap;
        if (node_open)
            ImGui::TreePop();
        
        // Display category
        node_flags = base_flags | (m_CurrentCategory == SettingsCategory::Display ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
        node_open = ImGui::TreeNodeEx("Display", node_flags);
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            m_CurrentCategory = SettingsCategory::Display;
        if (node_open)
            ImGui::TreePop();
        
        // Audio category
        node_flags = base_flags | (m_CurrentCategory == SettingsCategory::Audio ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
        node_open = ImGui::TreeNodeEx("Audio", node_flags);
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            m_CurrentCategory = SettingsCategory::Audio;
        if (node_open)
            ImGui::TreePop();
        
        // Physics category
        node_flags = base_flags | (m_CurrentCategory == SettingsCategory::Physics ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
        node_open = ImGui::TreeNodeEx("Physics", node_flags);
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            m_CurrentCategory = SettingsCategory::Physics;
        if (node_open)
            ImGui::TreePop();
        
        // Rendering category
        node_flags = base_flags | (m_CurrentCategory == SettingsCategory::Rendering ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
        node_open = ImGui::TreeNodeEx("Rendering", node_flags);
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            m_CurrentCategory = SettingsCategory::Rendering;
        if (node_open)
            ImGui::TreePop();
        
        ImGui::EndChild();
    }

    void ProjectSettingsPanel::RenderSettingsContent()
    {
        ImGui::SameLine();
        ImGui::BeginChild("SettingsContent", ImVec2(0, 0), true);
        
        switch (m_CurrentCategory)
        {
            case SettingsCategory::General:
                RenderGeneralSettings();
                break;
            case SettingsCategory::InputMap:
                RenderInputMapSettings();
                break;
            case SettingsCategory::Display:
                RenderDisplaySettings();
                break;
            case SettingsCategory::Audio:
                RenderAudioSettings();
                break;
            case SettingsCategory::Physics:
                RenderPhysicsSettings();
                break;
            case SettingsCategory::Rendering:
                RenderRenderingSettings();
                break;
            default:
                break;
        }
        
        ImGui::EndChild();
    }

    void ProjectSettingsPanel::RenderInputActionsList()
    {
        ImGui::BeginChild("ActionsList", ImVec2(300, -30), true);
        
        auto& bindings = Input::GetAllBindings();
        
        ImGui::Text("Actions");
        ImGui::Separator();
        
        ImGuiListClipper clipper;
        clipper.Begin(static_cast<int>(bindings.size()));
        
        int index = 0;
        for (auto& [actionName, binding] : bindings)
        {
            if (clipper.Step())
            {
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                {
                    auto it = bindings.begin();
                    std::advance(it, i);
                    
                    ImGui::PushID(it->first.c_str());
                    
                    bool isSelected = (m_SelectedActionName == it->first);
                    if (ImGui::Selectable(it->first.c_str(), isSelected))
                    {
                        SetSelectedAction(it->first);
                    }
                    
                    ImGui::PopID();
                }
            }
            index++;
        }
        
        ImGui::EndChild();
        
        // Add/Remove buttons
        if (ImGui::Button("Add", ImVec2(145, 0)))
        {
            m_ShowAddActionPopup = true;
            m_NewActionNameBuffer.fill('\0');
        }
        
        ImGui::SameLine();
        
        if (ImGui::Button("Remove", ImVec2(145, 0)))
        {
            if (!m_SelectedActionName.empty())
            {
                bindings.erase(m_SelectedActionName);
                m_SelectedActionName.clear();
                m_SelectedAction = nullptr;
            }
        }
    }

    void ProjectSettingsPanel::RenderAddActionPopup()
    {
        if (m_ShowAddActionPopup)
        {
            ImGui::OpenPopup("Add Action");
            m_ShowAddActionPopup = false;
        }
        
        if (ImGui::BeginPopupModal("Add Action", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Enter action name:");
            ImGui::SetNextItemWidth(300);
            
            bool enterPressed = ImGui::InputText("##NewActionName", m_NewActionNameBuffer.data(), m_NewActionNameBuffer.size(), ImGuiInputTextFlags_EnterReturnsTrue);
            
            ImGui::Separator();
            
            if (ImGui::Button("OK", ImVec2(145, 0)) || enterPressed)
            {
                std::string newActionName(m_NewActionNameBuffer.data());
                if (!newActionName.empty())
                {
                    auto& bindings = Input::GetAllBindings();
                    if (bindings.find(newActionName) == bindings.end())
                    {
                        bindings[newActionName] = InputBinding();
                        bindings[newActionName].Name = newActionName;
                        SetSelectedAction(newActionName);
                    }
                }
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            
            if (ImGui::Button("Cancel", ImVec2(145, 0)))
            {
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
    }

    void ProjectSettingsPanel::RenderInputActionDetails()
    {
        ImGui::SameLine();
        ImGui::BeginChild("ActionDetails", ImVec2(0, 0), true);
        
        if (!m_SelectedAction)
        {
            ImGui::TextDisabled("No action selected");
            ImGui::EndChild();
            return;
        }
        
        auto& bindings = Input::GetAllBindings();
        
        ImGui::Text("Action: %s", m_SelectedActionName.c_str());
        ImGui::Separator();
        
        // Rename action
        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        if (ImGui::InputText("##RenameAction", m_RenameActionBuffer.data(), m_RenameActionBuffer.size(), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            std::string newName(m_RenameActionBuffer.data());
            if (!newName.empty() && newName != m_SelectedActionName)
            {
                // Rename the action
                InputBinding temp = *m_SelectedAction;
                temp.Name = newName;
                bindings.erase(m_SelectedActionName);
                bindings[newName] = temp;
                SetSelectedAction(newName);
            }
        }
        
        ImGui::Spacing();
        ImGui::Text("Bindings:");
        ImGui::Separator();
        
        // Positive Key
        ImGui::Text("Positive Key:");
        ImGui::SameLine(150);
        ImGui::Text("%s", Input::GetKeyLabel(m_SelectedAction->KeyPos));
        ImGui::SameLine();
        if (ImGui::Button("Set##PosKey"))
        {
            Input::StartRebindMode(m_SelectedActionName, RebindState::PosKey);
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear##PosKey"))
        {
            m_SelectedAction->KeyPos = Key::Unknown;
        }
        
        // Negative Key
        ImGui::Text("Negative Key:");
        ImGui::SameLine(150);
        ImGui::Text("%s", Input::GetKeyLabel(m_SelectedAction->KeyNeg));
        ImGui::SameLine();
        if (ImGui::Button("Set##NegKey"))
        {
            Input::StartRebindMode(m_SelectedActionName, RebindState::NegKey);
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear##NegKey"))
        {
            m_SelectedAction->KeyNeg = Key::Unknown;
        }
        
        ImGui::Spacing();
        
        // Positive Button
        ImGui::Text("Positive Button:");
        ImGui::SameLine(150);
        ImGui::Text("%s", Input::GetButtonLabel(m_SelectedAction->ButtonPos));
        ImGui::SameLine();
        if (ImGui::Button("Set##PosButton"))
        {
            Input::StartRebindMode(m_SelectedActionName, RebindState::PosButton);
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear##PosButton"))
        {
            m_SelectedAction->ButtonPos = Button::Invalid;
        }
        
        // Negative Button
        ImGui::Text("Negative Button:");
        ImGui::SameLine(150);
        ImGui::Text("%s", Input::GetButtonLabel(m_SelectedAction->ButtonNeg));
        ImGui::SameLine();
        if (ImGui::Button("Set##NegButton"))
        {
            Input::StartRebindMode(m_SelectedActionName, RebindState::NegButton);
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear##NegButton"))
        {
            m_SelectedAction->ButtonNeg = Button::Invalid;
        }
        
        ImGui::Spacing();
        
        // Axis
        ImGui::Text("Axis:");
        ImGui::SameLine(150);
        ImGui::Text("%s", Input::GetAxisLabel(m_SelectedAction->Axis));
        ImGui::SameLine();
        if (ImGui::Button("Set##Axis"))
        {
            Input::StartRebindMode(m_SelectedActionName, RebindState::Axis);
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear##Axis"))
        {
            m_SelectedAction->Axis = Axis::Invalid;
            m_SelectedAction->invertedAxis = false;
        }
        
        ImGui::Checkbox("Inverted Axis", &m_SelectedAction->invertedAxis);
        
        ImGui::Spacing();
        ImGui::Separator();
        
        // Current values (for debugging)
        if (ImGui::CollapsingHeader("Debug Values"))
        {
            ImGui::Text("Positive Key Value: %d", Input::IsKeyPressed(m_SelectedAction->KeyPos));
            ImGui::Text("Negative Key Value: %d", Input::IsKeyPressed(m_SelectedAction->KeyNeg));
            ImGui::Text("Positive Button Value: %d", Input::GetButtonRaw(m_SelectedAction->ButtonPos));
            ImGui::Text("Negative Button Value: %d", Input::GetButtonRaw(m_SelectedAction->ButtonNeg));
            ImGui::Text("Axis Value: %d", Input::GetAxisRaw(m_SelectedAction->Axis));
        }
        
        ImGui::EndChild();
    }

    void ProjectSettingsPanel::RenderInputMapSettings()
    {
        ImGui::Text("Input Map");
        ImGui::SameLine();
        if (ImGui::Button("Save"))
        {
            Input::Save();
        }
        ImGui::Separator();
        
        RenderInputActionsList();
        RenderInputActionDetails();
        RenderAddActionPopup();

    }

    void ProjectSettingsPanel::RenderGeneralSettings()
    {
        ImGui::Text("General");
        ImGui::Separator();
        
        ImGui::Spacing();
        
        // Project Name
        ImGui::Text("Project Name:");
        ImGui::SameLine(150);
        Ref<Project> project = Project::GetActive();
        if (project)
        {
            ImGui::TextDisabled("%s", project->GetProjectName().c_str());
        }
        
        ImGui::Spacing();
        
        // Audio Directory
        ImGui::Text("Audio Banks Path:");
        std::string audioPath = Coffee::Project::GetRelativeAudioDirectory().string();
        ImGui::SameLine(150);
        ImGui::SetNextItemWidth(300);
        ImGui::InputText("##AudioBanksPath", audioPath.data(), audioPath.size(), ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine();
        if (ImGui::Button("Browse..."))
        {
            FileDialogArgs args;
            args.DefaultPath = Project::GetProjectDirectory().string();
            std::filesystem::path path = FileDialog::PickFolder(args);
            if (is_directory(path))
            {
                path = std::filesystem::relative(path, Project::GetProjectDirectory());
                Project::SetRelativeAudioDirectory(path);
                Audio::OnProjectLoad();
            }
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextDisabled("More settings coming soon...");
    }
    
    void ProjectSettingsPanel::RenderDisplaySettings()
    {
        ImGui::Text("Display Settings");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::TextDisabled("Display settings coming soon...");
    }
    
    void ProjectSettingsPanel::RenderAudioSettings()
    {
        ImGui::Text("Audio Settings");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::TextDisabled("Audio settings coming soon...");
    }
    
    void ProjectSettingsPanel::RenderPhysicsSettings()
    {
        ImGui::Text("Physics Settings");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::TextDisabled("Physics settings coming soon...");
    }
    
    void ProjectSettingsPanel::RenderRenderingSettings()
    {
        ImGui::Text("Rendering Settings");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::TextDisabled("Rendering settings coming soon...");
    }

    void ProjectSettingsPanel::OnImGuiRender()
    {
        if (!m_Visible) return;

        Ref<Project> project = Project::GetActive();

        if (!project)
        {
            ImGui::Begin("Project Settings", &m_Visible);
            ImGui::Text("No project loaded");
            ImGui::End();
            return;
        }

        ImGui::SetNextWindowSize({1024, 600}, ImGuiCond_FirstUseEver);

        if (ImGui::Begin("Project Settings", &m_Visible))
        {
            // Search bar at the top
            RenderSearchBar();
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            
            // Main layout: category tree on left, settings content on right
            RenderCategoryTree();
            RenderSettingsContent();
        }
        ImGui::End();
    }

} // Coffee