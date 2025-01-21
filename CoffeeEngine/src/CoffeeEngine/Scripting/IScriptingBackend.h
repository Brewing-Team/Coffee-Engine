#pragma once

#include <filesystem>
namespace Coffee {

    template<typename DerivedScript>
    class Script;
    
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

            template<typename DerivedScript>
            Script<DerivedScript> CreateScript(const std::filesystem::path& path)
            {
                return static_cast<DerivedScript*>(this)->CreateScript(path);
            }

            template<typename DerivedScript>
            void ExecuteScript(const Script<DerivedScript>& script)
            {
                //static_cast<DerivedScript*>(this)->ExecuteScript(script);
            }

            virtual void Shutdown() = 0;
    };

} // namespace Coffee