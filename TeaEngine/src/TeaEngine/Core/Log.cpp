#include "TeaEngine/Core/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace Tea
{
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
    std::vector<std::string> Log::s_LogBuffer;

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        auto imgui_sink = std::make_shared<LogSink<std::mutex>>();
        imgui_sink->set_level(spdlog::level::trace);

        s_CoreLogger = std::make_shared<spdlog::logger>("CORE", imgui_sink);
        s_CoreLogger->set_level(spdlog::level::trace);

        s_ClientLogger = std::make_shared<spdlog::logger>("APP", imgui_sink);
        s_ClientLogger->set_level(spdlog::level::trace);

        spdlog::register_logger(s_CoreLogger);
        spdlog::register_logger(s_ClientLogger);
    }

} // namespace Tea
