#pragma once
#include <filesystem>
#include <functional>
#include <sol/forward.hpp>
#include <string>

namespace Coffee {

    class IScriptingBackend {

        public:
            /**
             * @brief Virtual destructor for the IScriptingBackend interface.
             */
            virtual ~IScriptingBackend() = default;

            /**
             * @brief Initializes the scripting backend.
             */
            virtual void Initialize() = 0;

            /**
             * @brief Executes a script.
             *
             * @param script The script to execute.
             */
            virtual void ExecuteScript(const std::string& script) = 0;

            virtual void ExecuteFile(const std::filesystem::path& filepath) = 0;

            /**
             * @brief Registers a function with the scripting backend.
             *
             * @param name The name of the function.
             * @param func The function to register.
             */
            virtual void RegisterFunction(const std::string& script, std::function<int()> func, const std::string& name) = 0;

            virtual void BindFunction(const std::string& script, const std::string& name, sol::protected_function& func) = 0;

            virtual void RegisterVariable(const std::string& name, void* variable) = 0;

    };

} // namespace Coffee