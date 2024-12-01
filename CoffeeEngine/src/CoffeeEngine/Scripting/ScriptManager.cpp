#include "ScriptManager.h"

#include "Script.h"

namespace Coffee {

    std::unordered_map<ScriptingLanguage, std::shared_ptr<IScriptingBackend>> ScriptManager::backends;

    void ScriptManager::RegisterBackend(ScriptingLanguage language, std::shared_ptr<IScriptingBackend> backend) {
        backends[language] = backend;
        backend->Initialize();
    }

    void ScriptManager::ExecuteScriptFromFile(Script script) {
        backends[script.GetLanguage()]->ExecuteFile(script.GetPath());
    }

    void ScriptManager::RegisterFunction(const std::string& name, std::function<void()> func) {
        for(auto& backend : backends) {
            backend.second->RegisterFunction(func, name);
        }
    }

    void ScriptManager::BindFunction(const std::string& name, std::function<void()>& func) {
        for(auto& backend : backends) {
            backend.second->BindFunction(name, func);
        }
    }

    void ScriptManager::RegisterVariable(const std::string& name, void* variable) {
        for(auto& backend : backends) {
            backend.second->RegisterVariable(name, variable);
        }
    }

} // namespace Coffee