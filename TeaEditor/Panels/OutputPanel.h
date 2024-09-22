//
// Created by mdoradom on 20/09/24.
//

#pragma once
#include "Panel.h"
#include "imgui.h"

namespace Tea {

  class OutputPanel : public Panel {
  public:
    OutputPanel() = default;

    void OnImGuiRender() override;

  private:
    /**
     * Parses a log message into its components.
     *
     * @param log The log message to parse.
     * @return A tuple containing the parts of the log message.
     */
    std::tuple<std::string, std::string, std::string> ParseLogMessage(const std::string& log);

    /**
     * Renders a log message in the output panel.
     *
     * @param before_level The part of the message before the log level.
     * @param level_str The string representation of the log level.
     * @param after_level The part of the message after the log level.
     * @param level The log level enum.
     */
    void RenderLogMessage(const std::string& before_level, const std::string& level_str, const std::string& after_level,
                          spdlog::level::level_enum level);

    /**
     * Gets the color associated with a log level.
     *
     * @param level The log level enum.
     * @return The color associated with the log level.
     */
    ImVec4 GetLogLevelColor(spdlog::level::level_enum level);

  };

} // Tea