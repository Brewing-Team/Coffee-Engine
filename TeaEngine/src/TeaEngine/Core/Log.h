#pragma once

#include <memory>
#include <spdlog/logger.h>

namespace Tea
{
    class Log
    {
      public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

      private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
} // namespace Tea

// Core log macros
#define TEA_CORE_TRACE(...) ::Tea::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TEA_CORE_INFO(...)  ::Tea::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TEA_CORE_WARN(...)  ::Tea::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TEA_CORE_ERROR(...) ::Tea::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TEA_CORE_FATAL(...) ::Tea::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define TEA_TRACE(...)      ::Tea::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TEA_INFO(...)       ::Tea::Log::GetClientLogger()->info(__VA_ARGS__)
#define TEA_WARN(...)       ::Tea::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TEA_ERROR(...)      ::Tea::Log::GetClientLogger()->error(__VA_ARGS__)
#define TEA_FATAL(...)      ::Tea::Log::GetClientLogger()->fatal(__VA_ARGS__)