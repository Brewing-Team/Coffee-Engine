#include "MonitorPanel.h"
#include <SDL3/SDL_timer.h>
#include <cstdlib>
#include <imgui.h>

namespace Coffee {

    void MonitorPanel::OnImGuiRender()
    {
        ImGui::Begin("Monitor");

        // Set up two columns
        ImGui::Columns(2, "MonitorColumns");

        // First column

        // Header for the first column
        ImGui::BeginChild("LeftColumn", {0,0}, ImGuiChildFlags_Border);
        if(ImGui::BeginTable("HeaderTable", 2))
        {
            ImGui::TableSetupColumn("HeaderColumn1");
            ImGui::TableSetupColumn("HeaderColumn2");
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Monitor");
            ImGui::TableNextColumn();
            ImGui::Text("Value");
            ImGui::EndTable();
        }

        // Time
        if (ImGui::TreeNode("Time")) {
            ImGui::BeginTable("TimeTable", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_RowBg);
            ImGui::TableSetupColumn("TimeColumn1", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("TimeColumn2", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Checkbox("FPS", &m_ShowFPS);
            ImGui::TableNextColumn();
            ImGui::Text("60");
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Checkbox("Frame Time", &m_ShowFrameTime);
            ImGui::TableNextColumn();
            ImGui::Text("16.67 ms");
            ImGui::EndTable();
            ImGui::TreePop();
        }
        // Memory
        if(ImGui::TreeNode("Memory")) {
            ImGui::BeginTable("MemoryTable", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_RowBg);
            ImGui::TableSetupColumn("MemoryColumn1", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("MemoryColumn2", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Checkbox("Memory Usage", &m_MemoryUsage);
            ImGui::TableNextColumn();
            ImGui::Text("512 MB");
            ImGui::EndTable();
            ImGui::TreePop();
        }
        ImGui::EndChild();

        ImGui::NextColumn();

        ImGui::BeginChild("RightColumn", {0,0}, ImGuiChildFlags_Border);
        // Second column
        if (m_ShowFPS)
        {
            ImGui::Text("FPS");
            ImGui::PlotLines("##FPS", [](void* data, int idx) -> float {
                return sin(SDL_GetTicks() / 1000.0f + idx * 0.1f) * 100.0f + 100.0f;
            }, NULL, 100, 0, nullptr, FLT_MIN, FLT_MAX, ImVec2(0, 80)); // Minimum height of 80
        }

        if (m_ShowFrameTime)
        {
            ImGui::Text("Frame Time");
            ImGui::PlotLines("##FrameTime", [](void* data, int idx) -> float {
                return 16.67f;
            }, NULL, 100, 0, "Frame Time: %.2f ms", FLT_MIN, FLT_MAX, ImVec2(0, 80)); // Minimum height of 80
        }

        if (m_MemoryUsage)
        {
            ImGui::Text("Memory Usage");
            ImGui::PlotLines("##MemoryUsage", [](void* data, int idx) -> float {
                return sin(SDL_GetTicks() / 1000.0f + idx * 0.1f) * 100.0f + 100.0f;
            }, NULL, 100, 0, "Memory Usage: %.2f MB", FLT_MIN, FLT_MAX, ImVec2(0, 80)); // Minimum height of 80
        }
        ImGui::EndChild();

        ImGui::End();
    }

}