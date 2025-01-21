#include "ScriptManager.h"

#include "Script.h"

namespace Coffee {

    std::unordered_map<ScriptingLanguage, Scope<IScriptingBackend>> ScriptManager::backends;

    void ScriptManager::RegisterBackend(ScriptingLanguage language, Scope<IScriptingBackend> backend) {
        backends[language] = std::move(backend);
        backend->Initialize();
    }

    void ScriptManager::RemoveBackend(ScriptingLanguage language) {
        backends[language]->Shutdown();
        backends.erase(language);
    }

} // namespace Coffee