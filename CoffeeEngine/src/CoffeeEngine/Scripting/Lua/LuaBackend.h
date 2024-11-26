#pragma once
#include "CoffeeEngine/Scripting/IScriptingBackend.h"

#include <functional>
#include <string>
#include <sol/sol.hpp>

namespace Coffee {

    class LuaBackend : public IScriptingBackend {

        public:
            void Initialize() override;
            void ExecuteScript(const std::string& script) override;
            void RegisterFunction(const std::string& name, std::function<void()> func) override;
        private:
            sol::state luaState;

    };

} // namespace Coffee