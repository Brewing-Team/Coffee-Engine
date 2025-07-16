#include "ProjectSettingsPanel.h"

#include "CoffeeEngine/Core/FileDialog.h"
#include <imgui.h>

#include "CoffeeEngine/Core/Application.h"
#include "CoffeeEngine/Core/FileDialog.h"
#include "CoffeeEngine/Core/Input.h"

namespace Coffee {

    void ProjectSettingsPanel::BeginHorizontalChild(const char* label, const ImGuiWindowFlags flags)
    {
        ImGui::SameLine();
        ImGui::BeginChild(label);
    }

    void ProjectSettingsPanel::SetSelectedBinding(std::string actionName, InputBinding* binding)
    {
        m_SelectedInputKey = actionName;
        m_SelectedInputBinding = binding;
        // Copy name to modifiable array to enable Input Action name change
        std::copy_n(m_SelectedInputKey.begin(), std::min<size_t>(m_SelectedInputKey.size(), 255), arr_newBindName.begin());

    }
    void ProjectSettingsPanel::RenderInputSettings(const ImGuiWindowFlags flags)
    {
        bool isInputPanelVisible = (m_VisiblePanels & PanelDisplayEnum::Input);
        Application::Get().GetImGuiLayer()->BlockEvents(!isInputPanelVisible);

        if (!isInputPanelVisible) return;

        BeginHorizontalChild("Input", flags);
        ImGui::Text("Input Settings");
        ImGui::SameLine();
        if (ImGui::Button("Save Inputs"))
        {
            Input::Save();
        }

#pragma region InputMap
        auto& bindings = Input::GetAllBindings();
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

        if (ImGui::BeginTable("InputMap", 4, ImGuiTableFlags_ScrollY, {480, 240}))
        {


            ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Bound to", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_None);
            ImGui::TableHeadersRow();

            for (auto& binding : bindings)
            {
                ImGui::PushID(binding.first.c_str());
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                if (ImGui::TreeNodeEx(binding.first.c_str(), node_flags))
                {
                    InputBinding& b = binding.second;

                    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                    {
                        SetSelectedBinding(binding.first, &binding.second);
                    }


                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    if (ImGui::TreeNodeEx("PosButton", ImGuiTreeNodeFlags_Leaf))
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text("Button");

                        ImGui::TableNextColumn();
                        ImGui::Text("%s", Input::GetButtonLabel(b.ButtonPos));

                        ImGui::TableNextColumn();
                        ImGui::Text("%i", Input::GetButtonRaw(b.ButtonPos));

                        ImGui::TableNextColumn();
                        ImGui::TreePop();
                    }
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    if (ImGui::TreeNodeEx("NegButton", ImGuiTreeNodeFlags_Leaf))
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text("Button");

                        ImGui::TableNextColumn();
                        ImGui::Text("%s", Input::GetButtonLabel(b.ButtonNeg));

                        ImGui::TableNextColumn();
                        ImGui::Text("%i", Input::GetButtonRaw(b.ButtonNeg));

                        ImGui::TableNextColumn();
                        ImGui::TreePop();
                    }
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    if (ImGui::TreeNodeEx("PosKey", ImGuiTreeNodeFlags_Leaf))
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text("Key");

                        ImGui::TableNextColumn();
                        ImGui::Text("%s", Input::GetKeyLabel(b.KeyPos));

                        ImGui::TableNextColumn();
                        ImGui::Text("%i", Input::IsKeyPressed(b.KeyPos));

                        ImGui::TableNextColumn();
                        ImGui::TreePop();
                    }
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    if (ImGui::TreeNodeEx("NegKey", ImGuiTreeNodeFlags_Leaf))
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text("Key");

                        ImGui::TableNextColumn();
                        ImGui::Text("%s", Input::GetKeyLabel(b.KeyNeg));

                        ImGui::TableNextColumn();
                        ImGui::Text("%i", Input::IsKeyPressed(b.KeyNeg));

                        ImGui::TableNextColumn();
                        ImGui::TreePop();
                    }
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    if (ImGui::TreeNodeEx("Axis", ImGuiTreeNodeFlags_Leaf))
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text("Axis");

                        ImGui::TableNextColumn();
                        ImGui::Text( "%s", Input::GetAxisLabel(b.Axis));

                        ImGui::TableNextColumn();
                        ImGui::Text("%i", Input::GetAxisRaw(b.Axis));

                        ImGui::TableNextColumn();
                        ImGui::TreePop();
                    }

                    ImGui::TreePop();
                }
                ImGui::PopID();
            }
            ImGui::EndTable();

        }
#pragma endregion InputMap


        ImGui::SameLine();
        ImGui::Separator();

        ImGui::SameLine();
        ImGui::PushID("BindingConfig");
        ImGui::BeginGroup();


        if (m_SelectedInputBinding)
        {
            ImGui::TextUnformatted("Name: "); ImGui::SameLine();
            if (ImGui::InputText("BindingName", arr_newBindName.data(), 255, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                std::string str_newBindName = {arr_newBindName.begin(), arr_newBindName.end()};
                if (strlen(arr_newBindName.data()) != 0)
                {
                    m_SelectedInputBinding->Name = str_newBindName;
                    bindings[str_newBindName] = *m_SelectedInputBinding;
                    bindings.erase(m_SelectedInputKey);
                    m_SelectedInputBinding = &bindings[str_newBindName];
                    m_SelectedInputKey = str_newBindName;
                }
            }
            ImGui::NewLine();
            ImGui::TextUnformatted("PosButton:"); ImGui::SameLine();
            ImGui::Text("%s", Input::GetButtonLabel(m_SelectedInputBinding->ButtonPos)); ImGui::SameLine();
            if (ImGui::Button("Rebind##PosButton"))
            {
                Input::StartRebindMode(m_SelectedInputKey, RebindState::PosButton);
            }
            ImGui::SameLine();
            if (ImGui::Button("Unbind##PosButtonUnBind"))
            {
                m_SelectedInputBinding->ButtonPos = Button::Invalid;
            }

            ImGui::TextUnformatted("NegButton:"); ImGui::SameLine();
            ImGui::Text("%s", Input::GetButtonLabel(m_SelectedInputBinding->ButtonNeg)); ImGui::SameLine();
            if (ImGui::Button("Rebind##NegButton"))
            {
                Input::StartRebindMode(m_SelectedInputKey, RebindState::NegButton);
            }
            ImGui::SameLine();
            if (ImGui::Button("Unbind##NegButtonUnBind"))
            {
                m_SelectedInputBinding->ButtonNeg = Button::Invalid;
            }

            ImGui::TextUnformatted("PosKey:"); ImGui::SameLine();
            ImGui::Text("%s", Input::GetKeyLabel(m_SelectedInputBinding->KeyPos)); ImGui::SameLine();
            if (ImGui::Button("Rebind##PosKey"))
            {
                Input::StartRebindMode(m_SelectedInputKey, RebindState::PosKey);
            }
            ImGui::SameLine();
            if (ImGui::Button("Unbind##PosKeyUnBind"))
            {
                m_SelectedInputBinding->KeyPos = Key::Unknown;
            }

            ImGui::TextUnformatted("NegKey:"); ImGui::SameLine();
            ImGui::Text("%s", Input::GetKeyLabel(m_SelectedInputBinding->KeyNeg)); ImGui::SameLine();
            if (ImGui::Button("Rebind##NegKey"))
            {
                Input::StartRebindMode(m_SelectedInputKey, RebindState::NegKey);
            }
            ImGui::SameLine();
            if (ImGui::Button("Unbind##NegKeyUnBind"))
            {
                m_SelectedInputBinding->KeyNeg = Key::Unknown;
            }

            ImGui::TextUnformatted("Axis:"); ImGui::SameLine();
            ImGui::Text("%s", Input::GetAxisLabel(m_SelectedInputBinding->Axis)); ImGui::SameLine();
            if (ImGui::Button("Rebind##Axis"))
            {
                Input::StartRebindMode(m_SelectedInputKey, RebindState::Axis);
            }
            ImGui::SameLine();
            if (ImGui::Button("Unbind##AxisUnBind"))
            {
                m_SelectedInputBinding->Axis = Axis::Invalid;
                m_SelectedInputBinding->invertedAxis = false;
            }
            ImGui::Checkbox("Inverted axis", &m_SelectedInputBinding->invertedAxis);

            static bool deletionConfirmation = false;
            std::string deletionLabel = deletionConfirmation ? "Are you sure?":"Delete";
            deletionLabel += "##ActionDeleteButton";
            if (ImGui::Button(deletionLabel.c_str()))
            {
                if (!deletionConfirmation)
                    deletionConfirmation = true;
                else
                {
                    bindings.erase(m_SelectedInputKey);
                    deletionConfirmation = false;
                    m_SelectedInputKey = "";
                    m_SelectedInputBinding = nullptr;
                }
            }
        }
        else
        {
            arr_newBindName.fill('\0');
        }
        else
        {
            arr_newBindName.fill('\0');
        }

        if (ImGui::Button("New Action"))
        {
            bindings["NewAction"] = InputBinding();
            SetSelectedBinding("NewAction", &bindings["NewAction"]);
        }

        ImGui::EndGroup();
        ImGui::PopID();

        ImGui::EndChild();
    }

    void ProjectSettingsPanel::RenderGeneralSettings(const ImGuiWindowFlags flags)
    {
        if (!(m_VisiblePanels & PanelDisplayEnum::General))
            return;

        if (m_RefreshPanels & PanelDisplayEnum::General)
        {
            // Refresh values (in case of new project loaded)
            std::string s = Project::GetProjectName();
            m_NewProjectName.fill('\0');
            std::copy_n(s.begin(), min(s.size(), 255),m_NewProjectName.begin());

            m_RefreshPanels ^= PanelDisplayEnum::General;
        }

        BeginHorizontalChild("General", flags);

        ImGui::Text("General settings for this project (WIP)");
        std::string str = Coffee::Project::GetRelativeAudioDirectory().string();
        char* strData = str.data();
        ImGui::InputText("##AudioBanksPath", strData, str.size(), ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine();
        if (ImGui::Button("Select...##AudioBanksPathButton"))
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

        ImGui::EndChild();
    }

    void ProjectSettingsPanel::OnImGuiRender()
    {
        if (!m_Visible) return;

        Ref<Project> project = Project::GetActive();

        if (!project)
        {
            ImGui::Begin("Project Settings");
            ImGui::Text("No project loaded");
            ImGui::End();
            return;
        }

        ImGui::SetNextWindowSize({960,480}, ImGuiCond_Once);

        ImGui::Begin("Project Settings");
        ImGui::Text("Project Settings");

        constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

        ImGui::PushID("ProjectSettings");

        if (ImGui::TreeNodeEx("Project", ImGuiTreeNodeFlags_Leaf))
        {
            if (ImGui::IsItemClicked())
                SetPanelVisible(PanelDisplayEnum::General);
            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Input", ImGuiTreeNodeFlags_Leaf))
        {
            if (ImGui::IsItemClicked())
                SetPanelVisible(PanelDisplayEnum::Input);
            ImGui::TreePop();
        }

        ImGui::SameLine();
        ImGui::Separator();

        RenderGeneralSettings(flags);
        RenderInputSettings(flags);

        ImGui::PopID();
        ImGui::End();
    }

} // Coffee