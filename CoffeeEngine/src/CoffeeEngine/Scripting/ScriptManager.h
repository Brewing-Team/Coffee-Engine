#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "IScriptingBackend.h"
#include <filesystem>
#include <unordered_map>

namespace Coffee {

    enum class ScriptingLanguage {
        Lua,
        cSharp
    };

    class ScriptManager {
    public:
        static void RegisterBackend(ScriptingLanguage language, Scope<IScriptingBackend> backend);
        static void RemoveBackend(ScriptingLanguage language);

        template <typename DerivedScript>
        static Script<DerivedScript> CreateScript(const std::filesystem::path& path, ScriptingLanguage language) {
            return backends[language]->CreateScript<DerivedScript>(path);
        }

        template <typename DerivedScript>
        static void ExecuteScript(const Script<DerivedScript>& script, ScriptingLanguage language) {
            backends[language]->ExecuteScript(script);
        }

    private:
        static std::unordered_map<ScriptingLanguage, Scope<IScriptingBackend>> backends;
    };

} // namespace Coffee