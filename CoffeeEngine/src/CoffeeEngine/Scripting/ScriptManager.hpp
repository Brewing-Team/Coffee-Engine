#pragma once
#include "IScriptingBackend.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace Coffee {

    class ScriptManager {
        public:
            void RegisterBackend(const std::string& language, std::shared_ptr<IScriptingBackend> backend);
            void ExecuteScript(const std::string& language, const std::string& script);

        private:
            std::unordered_map<std::string, std::shared_ptr<IScriptingBackend>> backends;
    };

    inline void ScriptManager::RegisterBackend(const std::string& language, std::shared_ptr<IScriptingBackend> backend)
    {
        backends[language] = backend;
    }

    inline void ScriptManager::ExecuteScript(const std::string& language, const std::string& script) {
        if (backends.count(language)) {
            backends[language]->ExecuteScript(script);
        }
    }

} // namespace Coffee