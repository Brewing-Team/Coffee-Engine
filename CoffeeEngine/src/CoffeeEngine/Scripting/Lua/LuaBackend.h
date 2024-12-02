#pragma once
#include "CoffeeEngine/Scripting/IScriptingBackend.h"

#include <filesystem>
#include <functional>
#include <sol/sol.hpp>
#include <string>

namespace Coffee {

    struct LuaVariable {
        std::string name;
        std::string value;
    };

    class LuaBackend : public IScriptingBackend {

        public:
            void Initialize() override;
            void ExecuteScript(const std::string& script) override;
            void ExecuteFile(const std::filesystem::path& filepath) override;
            void RegisterFunction(std::function<void()> func, const std::string& name) override;
            void BindFunction(const std::string& name, std::function<void()>& func) override;
            void RegisterVariable(const std::string& name, void* variable) override;
            static std::vector<LuaVariable> MapVariables(const std::string& script);

          private:
            sol::state luaState;

    };

} // namespace Coffee