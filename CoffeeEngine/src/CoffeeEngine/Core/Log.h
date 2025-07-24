#pragma once

#include <spdlog/logger.h>
#include <spdlog/sinks/base_sink.h>

namespace Coffee
{
    /**
     * @defgroup core Core
     * @brief Core components of the CoffeeEngine.
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

        static const std::vector<std::string>& GetLogBuffer() { return s_LogBuffer; }
        static void ClearLogBuffer() { s_LogBuffer.clear(); }

      private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger; ///< The core logger.
        static std::shared_ptr<spdlog::logger> s_ClientLogger; ///< The client logger.
        static std::vector<std::string> s_LogBuffer; ///< The log buffer.

        template <typename Mutex>
        class LogSink : public spdlog::sinks::base_sink<Mutex>
        {
        protected:
            void sink_it_(const spdlog::details::log_msg& msg) override
            {
                if(s_LogBuffer.size() > 1024)
                {
                    s_LogBuffer.erase(s_LogBuffer.begin());
                }

                spdlog::memory_buf_t formatted;
                this->formatter_->format(msg, formatted);
                s_LogBuffer.push_back(fmt::to_string(formatted));
            }

            void flush_() override {}
        };
    };
    /** @} */
} // namespace Coffee

// Core log macros
#define COFFEE_CORE_TRACE(...)    ::Coffee::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define COFFEE_CORE_INFO(...)     ::Coffee::Log::GetCoreLogger()->info(__VA_ARGS__)
#define COFFEE_CORE_WARN(...)     ::Coffee::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define COFFEE_CORE_ERROR(...)    ::Coffee::Log::GetCoreLogger()->error(__VA_ARGS__)
#define COFFEE_CORE_CRITICAL(...) ::Coffee::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define COFFEE_TRACE(...)         ::Coffee::Log::GetClientLogger()->trace(__VA_ARGS__)
#define COFFEE_INFO(...)          ::Coffee::Log::GetClientLogger()->info(__VA_ARGS__)
#define COFFEE_WARN(...)          ::Coffee::Log::GetClientLogger()->warn(__VA_ARGS__)
#define COFFEE_ERROR(...)         ::Coffee::Log::GetClientLogger()->error(__VA_ARGS__)
#define COFFEE_CRITICAL(...)      ::Coffee::Log::GetClientLogger()->critical(__VA_ARGS__)