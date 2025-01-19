#pragma once
#include "CoffeeEngine/Core/Base.h"
#include "IScriptingBackend.h"

#include <memory>
#include <unordered_map>

namespace Coffee {

    class Script;

    enum class ScriptingLanguage {
        Lua,
        cSharp
    };

    class ScriptManager {
    public:
        static void RegisterBackend(ScriptingLanguage language, std::shared_ptr<IScriptingBackend> backend);
        static void RemoveBackend(ScriptingLanguage language);
    private:
        static std::unordered_map<ScriptingLanguage, Ref<IScriptingBackend>> backends;
    };

} // namespace Coffee