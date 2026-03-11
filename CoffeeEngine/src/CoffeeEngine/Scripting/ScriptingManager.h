#pragma once

#include "CoffeeEngine/Core/Base.h"
#include <filesystem>
#include <unordered_map>

namespace Coffee {

    class IScriptingBackend;

    enum class ScriptingLanguage {
        Lua,
        cSharp
    };

    class ScriptingManager {
    public:
        void RegisterBackend(ScriptingLanguage language, Ref<IScriptingBackend> backend);
        const IScriptingBackend& GetBackend(ScriptingLanguage language);
        void RemoveBackend(ScriptingLanguage language);

        Ref<Script> CreateScript(const std::filesystem::path& path, ScriptingLanguage language) {
            return m_Backends[language]->CreateScript(path);
        }

        void ExecuteScript(Script& script, ScriptingLanguage language) {
            m_Backends[language]->ExecuteScript(script);
        }

        void SetWorkingDirectory(const std::filesystem::path& path) {
            for (auto& backend : m_Backends) {
                backend.second->SetWorkingDirectory(path);
            }
        }

    private:
        std::unordered_map<ScriptingLanguage, Ref<IScriptingBackend>> m_Backends;
    };

} // namespace Coffee