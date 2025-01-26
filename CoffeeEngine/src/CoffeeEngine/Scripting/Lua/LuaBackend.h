#pragma once
#include "CoffeeEngine/Scripting/IScriptingBackend.h"

#include <sol/sol.hpp>
#include <string>

namespace Coffee {

    class LuaScript;

    class LuaBackend : public IScriptingBackend {
        public:
            void Initialize() override;
            
            Ref<Script> CreateScript(const std::filesystem::path& path) override;
            void ExecuteScript(const Script& script) override;

            sol::state& GetLuaState() const { return luaState; }

            void Shutdown() override {}
        private:
            static sol::state luaState;
    };

} // namespace Coffee