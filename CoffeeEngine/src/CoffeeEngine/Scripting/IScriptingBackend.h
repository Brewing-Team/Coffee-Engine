#pragma once
#include <functional>
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

            /**
             * @brief Registers a function with the scripting backend.
             *
             * @param name The name of the function.
             * @param func The function to register.
             */
            virtual void RegisterFunction(const std::string& name, std::function<void()> func) = 0;

    };

} // namespace Coffee