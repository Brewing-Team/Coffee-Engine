#pragma once
#include "CoffeeEngine/Scripting/IScriptingBackend.h"

#include <sol/sol.hpp>
#include <string>

namespace Coffee {

    class LuaScript;

    struct LuaVariable {
        std::string name;
        std::string value;
        sol::type type;
    };

    class LuaBackend : public IScriptingBackend {
        public:
            void Initialize() override;
            
            Ref<Script> CreateScript(const std::filesystem::path& path) override;
            void ExecuteScript(const Script& script) override;

            sol::state& GetLuaState() const { return luaState; }

            void Shutdown() override {}
        private:
            static sol::state luaState;
            static std::vector<LuaVariable> MapVariables(const LuaScript& script);
    };

} // namespace Coffee