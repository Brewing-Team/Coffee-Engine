#pragma once

#include <memory>
#include <spdlog/logger.h>

namespace Tea
{
    /**
     * @defgroup core Core
     * @brief Core components of the TeaEngine.
     * @{
     */

    /**
     * @brief The Log class is responsible for initializing and providing access to the core and client loggers.
     */
    class Log
    {
      public:
        /**
         * @brief Initializes the logging system.
         */
        static void Init();

        /**
         * @brief Gets the core logger.
         * @return A shared pointer to the core logger.
         */
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

        /**
         * @brief Gets the client logger.
         * @return A shared pointer to the client logger.
         */
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

      private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger; ///< The core logger.
        static std::shared_ptr<spdlog::logger> s_ClientLogger; ///< The client logger.
    };
    /** @} */
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