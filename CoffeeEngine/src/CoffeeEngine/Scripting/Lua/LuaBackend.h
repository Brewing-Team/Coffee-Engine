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
            
            LuaScript CreateScript(const std::string& path);
            void ExecuteScript(const LuaScript& script);
        private:
            static sol::state luaState;
            static std::vector<LuaVariable> MapVariables(const LuaScript& script);
    };

} // namespace Coffee