#pragma once

#include "CoffeeEngine/Core/Base.h"
#include <filesystem>
#include <unordered_map>

namespace Coffee {

    class IScriptingBackend;
    class Script;

    enum class ScriptingLanguage {
        Lua,
        cSharp
    };

    class ScriptingManager {
    public:
        void RegisterBackend(ScriptingLanguage language, Ref<IScriptingBackend> backend);
        const IScriptingBackend& GetBackend(ScriptingLanguage language);
        void RemoveBackend(ScriptingLanguage language);

        Ref<Script> CreateScript(const std::filesystem::path& path, ScriptingLanguage language);

        void ExecuteScript(Script& script, ScriptingLanguage language);

        void SetWorkingDirectory(const std::filesystem::path& path);

    private:
        std::unordered_map<ScriptingLanguage, Ref<IScriptingBackend>> m_Backends;
    };

} // namespace Coffee