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

    void ScriptManager::RegisterFunction(const std::string& script, const std::string& name, std::function<int()> func) {
        for(auto& backend : backends) {
            backend.second->RegisterFunction(script, func, name);
        }
    }

    void ScriptManager::BindFunction(const std::string& script, const std::string& name, sol::protected_function& func) {
        for(auto& backend : backends) {
            backend.second->BindFunction(script, name, func);
        }
    }

    void ScriptManager::RegisterVariable(const std::string& name, void* variable) {
        for(auto& backend : backends) {
            backend.second->RegisterVariable(name, variable);
        }
    }

} // namespace Coffee