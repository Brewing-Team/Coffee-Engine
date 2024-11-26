#include "LuaBackend.h"

#include "CoffeeEngine/Core/Log.h"

namespace Coffee {

    void LuaBackend::Initialize() {
        luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);

        luaState["print_message"] = [](const std::string& message) {
            COFFEE_CORE_INFO("[Lua]: {0}", message);
        };
    }

    void LuaBackend::ExecuteScript(const std::string& script) {
        try {
            luaState.script(script);
        } catch (const sol::error& e) {
            COFFEE_CORE_ERROR("[Lua Error]: {0}", e.what());
        }
    }

    void LuaBackend::RegisterFunction(const std::string& name, std::function<void()> func) {
        luaState.set_function(name, func);
        COFFEE_CORE_INFO("Registered Lua function {0}", name);
    }

} // namespace Coffee