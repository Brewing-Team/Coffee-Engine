#include "ScriptingManager.h"

#include "Script.h"

namespace Coffee {

    void ScriptingManager::RegisterBackend(ScriptingLanguage language, Ref<IScriptingBackend> backend)
    {
        m_Backends[language] = backend;
        backend->Initialize();
    }

    const IScriptingBackend& ScriptingManager::GetBackend(ScriptingLanguage language)
    {
        return *m_Backends[language];
    }

    void ScriptingManager::RemoveBackend(ScriptingLanguage language)
    {
        m_Backends[language]->Shutdown();
        m_Backends.erase(language);
    }

    Ref<Script> ScriptingManager::CreateScript(const std::filesystem::path& path, ScriptingLanguage language)
    {
        return m_Backends[language]->CreateScript(path);
    }

    void ScriptingManager::ExecuteScript(Script& script, ScriptingLanguage language)
    {
        m_Backends[language]->ExecuteScript(script);
    }

    void ScriptingManager::SetWorkingDirectory(const std::filesystem::path& path)
    {
        for (auto& backend : m_Backends) {
            backend.second->SetWorkingDirectory(path);
        }
    }

} // namespace Coffee