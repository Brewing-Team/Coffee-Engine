#include "OutputPanel.h"
#include "CoffeeEngine/Core/Application.h"
#include "CoffeeEngine/Core/Log.h"
#include <imgui.h>
#include <spdlog/spdlog.h>

namespace Coffee {

    void OutputPanel::OnImGuiRender()
    {
        ImGui::Begin("Output");
        const auto& logBuffer = Coffee::Log::GetLogBuffer();
        for (const auto& log : logBuffer)
        {
            auto [before_level, level_str, after_level] = ParseLogMessage(log);
            spdlog::level::level_enum level = spdlog::level::from_str(level_str);
            RenderLogMessage(before_level, level_str, after_level, level);
        }
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
            case spdlog::level::trace: return ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // Gray
            case spdlog::level::debug: return ImVec4(0.0f, 1.0f, 1.0f, 1.0f); // Cyan
            case spdlog::level::info: return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);  // Green
            case spdlog::level::warn: return ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
            case spdlog::level::err: return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);  // Red
            case spdlog::level::critical: return ImVec4(1.0f, 0.0f, 1.0f, 1.0f); // Magenta
            default: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
        }
    }

} // namespace Coffee