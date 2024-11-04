#include "MonitorPanel.h"
#include "CoffeeEngine/Core/DataStructures/CircularBuffer.h"
#include "CoffeeEngine/Core/SystemInfo.h"
#include "CoffeeEngine/Core/Application.h"
#include "CoffeeEngine/Core/Timer.h"
#include <imgui.h>
#include <iostream>

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
                return /* Application::Get().GetFPS() */0;
            }, NULL, 100, 0, nullptr, FLT_MIN, FLT_MAX, ImVec2(0, 80)); // Minimum height of 80
        }

        if (m_ShowFrameTime)
        {
            ImGui::Text("Frame Time");
            ImGui::PlotLines("##FrameTime", [](void* data, int idx) -> float {
                return /* Application::Get().GetFrameTime() */0;
            }, NULL, 100, 0, "Frame Time: %.2f ms", FLT_MIN, FLT_MAX, ImVec2(0, 80)); // Minimum height of 80
        }

        if (m_MemoryUsage)
        {
            ImGui::Text("Memory Usage");

            //Test if this is better or is better to update the memory usage every x time
            static CircularBuffer<uint64_t> memoryUsage(10000);

            static Timer timer(0.5f, true, false, [&]() {
                memoryUsage.push_back(SystemInfo::GetProcessMemoryUsage());
            });

            static float yMin = 0.0f;
            static float yMax = 0.0f;

            ImGui::PlotLines("##MemoryUsage", [](void* data, int idx) -> float {
                auto& memoryUsage = *(CircularBuffer<uint64_t>*)data;
                uint64_t& mu = memoryUsage[idx];
                if (mu > yMax - 100) yMax = mu + 100;
                return mu;
            }, &memoryUsage, memoryUsage.size(), 0, "Memory Usage: %.2f MB", yMin, yMax, ImVec2(0, 80)); // Minimum height of 80
        }
        ImGui::EndChild();

        ImGui::End();
    }

}