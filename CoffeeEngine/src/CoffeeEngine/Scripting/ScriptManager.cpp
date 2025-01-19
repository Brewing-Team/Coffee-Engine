#include "ScriptManager.h"

#include "Script.h"

namespace Coffee {

    std::unordered_map<ScriptingLanguage, Ref<IScriptingBackend>> ScriptManager::backends;

    void ScriptManager::RegisterBackend(ScriptingLanguage language, Ref<IScriptingBackend> backend) {
        backends[language] = backend;
        backend->Initialize();
    }

    void ScriptManager::RemoveBackend(ScriptingLanguage language) {
        
    }

} // namespace Coffee