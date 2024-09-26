#include "CoffeeEngine/Core/Log.h"

#include <memory>
#include <mutex>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

namespace Coffee
{
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
    std::vector<std::string> Log::s_LogBuffer;

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        auto imGuiSink = std::make_shared<LogSink<std::mutex>>();
        imGuiSink->set_level(spdlog::level::trace);

        s_CoreLogger = spdlog::stdout_color_mt("CORE");
        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger.get()->sinks().push_back(imGuiSink);

        s_ClientLogger = spdlog::stdout_color_mt("APP");
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger.get()->sinks().push_back(imGuiSink);
    }

} // namespace Coffee
