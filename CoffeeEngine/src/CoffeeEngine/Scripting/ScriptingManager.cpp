#include "ScriptingManager.h"

#include "Script.h"

namespace Coffee {

    void ScriptingManager::RegisterBackend(ScriptingLanguage language, Ref<IScriptingBackend> backend) {
        m_Backends[language] = backend;
        backend->Initialize();
    }

    const IScriptingBackend& ScriptingManager::GetBackend(ScriptingLanguage language) {
        return *m_Backends[language];
    }

    void ScriptingManager::RemoveBackend(ScriptingLanguage language) {
        m_Backends[language]->Shutdown();
        m_Backends.erase(language);
    }

} // namespace Coffee