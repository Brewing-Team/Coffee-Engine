#pragma once
#include "CoffeeEngine/Scripting/IScriptingBackend.h"

#include <filesystem>
#include <functional>
#include <sol/sol.hpp>
#include <string>
#include <fstream>
#include <regex>

namespace Coffee {

    struct LuaVariable {
        std::string name;
        std::string value;
        sol::type type;
    };

    class LuaBackend : public IScriptingBackend {

        public:
            void Initialize() override;
            void ExecuteScript(const std::string& script) override;
            void ExecuteFile(const std::filesystem::path& filepath) override;
            void RegisterFunction(const std::string& script, std::function<int()> func, const std::string& name) override;
            void BindFunction(const std::string& script, const std::string& name, sol::protected_function& func) override;
            void RegisterVariable(const std::string& name, void* variable) override;
            static std::vector<LuaVariable> MapVariables(const std::string& script);
            static sol::state luaState;
            static std::unordered_map<std::string, sol::environment> scriptEnvironments;
    };

} // namespace Coffee