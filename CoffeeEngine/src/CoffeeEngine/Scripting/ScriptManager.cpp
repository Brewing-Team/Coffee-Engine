#include "ScriptManager.h"

#include "Script.h"

namespace Coffee {

    std::unordered_map<ScriptingLanguage, Ref<IScriptingBackend>> ScriptManager::backends;

    void ScriptManager::RegisterBackend(ScriptingLanguage language, Ref<IScriptingBackend> backend) {
        backends[language] = backend;
        backend->Initialize();
    }

    const IScriptingBackend& ScriptManager::GetBackend(ScriptingLanguage language) {
        return *backends[language];
    }

    void ScriptManager::RemoveBackend(ScriptingLanguage language) {
        backends[language]->Shutdown();
        backends.erase(language);
    }

} // namespace Coffee