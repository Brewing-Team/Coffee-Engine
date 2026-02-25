#include "OutputPanel.h"
#include "CoffeeEngine/Core/Log.h"
#include "IconsLucide.h"
#include <cstdlib>
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

namespace Coffee {

    // Helper function to extract only the message text from a log entry
    std::string ExtractMessageText(const std::string& log) {
        // Find the last ']' which marks the end of the log level
        size_t lastBracket = log.find_last_of(']');
        if (lastBracket != std::string::npos && lastBracket + 1 < log.length()) {
            // Extract everything after the last bracket
            std::string message = log.substr(lastBracket + 1);
            // Trim leading whitespace
            size_t start = message.find_first_not_of(" \t");
            if (start != std::string::npos) {
                return message.substr(start);
            }
        }
        // Fallback to original log if parsing fails
        return log;
    }

    void OutputPanel::OnImGuiRender()
    {
        if (!m_Visible) return;

        ImGui::Begin("Output", nullptr);

        static bool clear = false;
        static bool copy = false;

        const std::vector<std::string>& logBuffer = Coffee::Log::GetLogBuffer();
        
        // Handle clear button
        if (clear)
        {
            Coffee::Log::ClearLogBuffer();
            clear = false;
        }

        // Handle copy button
        if (copy)
        {
            ImGui::LogToClipboard();
            for (const std::string& log : logBuffer)
            {
                if (m_Filter.IsActive() && !m_Filter.PassFilter(log.c_str()))
                    continue;
                
                ImGui::LogText("%s\n", log.c_str());
            }
            ImGui::LogFinish();
            copy = false;
        }

        // Fixed-size right panel for buttons (Godot-like layout)
        const float rightPanelWidth = 100.0f;
        const float availWidth = ImGui::GetContentRegionAvail().x;
        const float availHeight = ImGui::GetContentRegionAvail().y;
        const float searchBarHeight = ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y;
        const float leftPanelWidth = availWidth - rightPanelWidth - ImGui::GetStyle().ItemSpacing.x;
        const float mainAreaHeight = availHeight - searchBarHeight;

        // Left panel - Log output
        ImGui::BeginChild("OutputLog", ImVec2(leftPanelWidth, mainAreaHeight), true);
        
        // Enable text wrapping
        ImGui::PushTextWrapPos(0.0f);
        
        // Display logs with filtering
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
        int logIndex = 0;
        for (const std::string& log : logBuffer)
        {
            // Skip this log if it doesn't pass the filter
            if (m_Filter.IsActive() && !m_Filter.PassFilter(log.c_str()))
                continue;
            
            auto [before_level, level_str, after_level] = ParseLogMessage(log);
            spdlog::level::level_enum level = spdlog::level::from_str(level_str);
            
            // Extract only the message text (without timestamp and log level)
            std::string messageText = ExtractMessageText(log);
            
            // Create unique ID for this log entry
            ImGui::PushID(logIndex++);
            
            // Push color for the text
            ImGui::PushStyleColor(ImGuiCol_Text, GetLogLevelColor(level));
            
            // Calculate wrapped text size to make selectable area match
            ImVec2 textSize = ImGui::CalcTextSize(messageText.c_str(), nullptr, false, ImGui::GetContentRegionAvail().x);
            
            // Make each log line selectable with proper size
            ImGui::Selectable("##selectable", false, ImGuiSelectableFlags_AllowOverlap, ImVec2(0, textSize.y));
            
            // Right-click context menu for copying (must be called right after Selectable)
            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Copy"))
                {
                    // Copy the full log with all flags and timestamps
                    ImGui::SetClipboardText(log.c_str());
                }
                ImGui::EndPopup();
            }
            
            // Draw wrapped text on top of the selectable
            ImVec2 textPos = ImGui::GetItemRectMin();
            ImGui::SetCursorScreenPos(textPos);
            ImGui::TextWrapped("%s", messageText.c_str());
            
            ImGui::PopStyleColor();
            ImGui::PopID();
        }
        ImGui::PopStyleVar();
        ImGui::PopTextWrapPos();

        // Auto-scroll to bottom when enabled
        if (m_AutoScroll)
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();

        // Right panel - Buttons (fixed width, non-resizable)
        ImGui::SameLine();
        ImGui::BeginChild("OutputOptions", ImVec2(rightPanelWidth, mainAreaHeight), true);
        
        // Buttons stacked vertically
        if (ImGui::Button("Clear", ImVec2(-1, 0)))
            clear = true;
        
        if (ImGui::Button("Copy", ImVec2(-1, 0)))
            copy = true;
        
        ImGui::Separator();
        
        if (ImGui::Button("Options", ImVec2(-1, 0)))
            ImGui::OpenPopup("Options");

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &m_AutoScroll);
            ImGui::EndPopup();
        }
        
        ImGui::EndChild();

        // Search bar at the bottom, matching the width of the log panel
        m_Filter.Draw(ICON_LC_SEARCH " Filter", leftPanelWidth);

        ImGui::End();
    }

    std::tuple<std::string, std::string, std::string> OutputPanel::ParseLogMessage(const std::string& log)
    {
        auto level_start = log.find_last_of('[');
        auto level_end = log.find(']', level_start) + 1;
        std::string level_str = log.substr(level_start + 1, level_end - level_start - 2);

        std::string before_level = log.substr(0, level_start);
        std::string after_level = log.substr(level_end);

        return {before_level, level_str, after_level};
    }

    void OutputPanel::RenderLogMessage(const std::string& before_level, const std::string& level_str, const std::string& after_level, spdlog::level::level_enum level)
    {
        ImGui::TextUnformatted(before_level.c_str());
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::TextUnformatted("[");
        ImGui::SameLine(0.0f, 0.0f);

        ImVec4 color = GetLogLevelColor(level);

        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(level_str.c_str());
        ImGui::PopStyleColor();
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::TextUnformatted("]");
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::TextUnformatted(after_level.c_str());
    }

    ImVec4 OutputPanel::GetLogLevelColor(spdlog::level::level_enum level)
    {
        switch (level)
        {
            case spdlog::level::trace: return ImVec4(0.655f, 0.596f, 0.514f, 1.0f); // #A79883
            case spdlog::level::debug: return ImGui::GetStyle().Colors[ImGuiCol_Text]; // Default text color
            case spdlog::level::info: return ImVec4(0.592f, 0.588f, 0.098f, 1.0f);  // #979619
            case spdlog::level::warn: return ImVec4(0.976f, 0.737f, 0.180f, 1.0f); // #F9BC2E
            case spdlog::level::err: return ImVec4(0.984f, 0.357f, 0.282f, 1.0f);  // #FB5B48
            case spdlog::level::critical: return ImVec4(0.796f, 0.137f, 0.110f, 1.0f); // #CB231C
            default: return ImGui::GetStyle().Colors[ImGuiCol_Text]; // Default text color
        }
    }

} // namespace Coffee