#include "ProjectSettingsPanel.h"
#include "CoffeeEngine/Project/Project.h"
#include <imgui.h>

#include "CoffeeEngine/Core/Input.h"


namespace Coffee {

    void ProjectSettingsPanel::BeginHorizontalChild(const char* label, const ImGuiWindowFlags flags)
    {
        ImGui::SameLine();
        ImGui::BeginChild(label);
    }

    void ProjectSettingsPanel::RenderInputSettings(const ImGuiWindowFlags flags)
    {
        if (!(m_VisiblePanels & PanelDisplayEnum::Input))
            return;

        BeginHorizontalChild("Input", flags);
        ImGui::Text("Input Settings");

#pragma region InputMap
        auto& bindings = Input::GetAllBindings();
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

        ImGui::BeginTable("InputMap", 4, ImGuiTableFlags_ScrollY);

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

                //ImGui::TableNextColumn();
                //ImGui::TextUnformatted("---");
                //ImGui::TableNextColumn();
                //ImGui::TextUnformatted("---");
                //ImGui::TableNextColumn();
                //ImGui::TextUnformatted("---");

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
#pragma endregion InputMap

        ImGui::EndChild();
    }

    void ProjectSettingsPanel::RenderGeneralSettings(const ImGuiWindowFlags flags)
    {
        if (!(m_VisiblePanels & PanelDisplayEnum::General))
            return;

        BeginHorizontalChild("General", flags);

        ImGui::Text("General settings for this project (WIP)");

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

        ImGui::Begin("Project Settings");
        ImGui::Text("Project Settings");

        constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

        ImGui::PushID("ProjectSettings");

        if (ImGui::TreeNodeEx("Project", ImGuiTreeNodeFlags_Leaf, ""))
        {
            if (ImGui::IsItemClicked())
                m_VisiblePanels = PanelDisplayEnum::General;
            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Input", ImGuiTreeNodeFlags_Leaf))
        {
            if (ImGui::IsItemClicked())
                m_VisiblePanels = PanelDisplayEnum::Input;
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