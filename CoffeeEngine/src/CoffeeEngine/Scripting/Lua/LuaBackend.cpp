#include "LuaBackend.h"

#include "CoffeeEngine/Core/Log.h"

namespace Coffee {

    void LuaBackend::Initialize() {
        luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
    }

    void LuaBackend::ExecuteScript(const std::string& script) {
        try {
            luaState.script(script);
        } catch (const sol::error& e) {
            COFFEE_CORE_ERROR("[Lua Error]: {0}", e.what());
        }
    }

    void LuaBackend::ExecuteFile(const std::filesystem::path& filepath) {
        try {
            luaState.script_file(filepath.string());
        } catch (const sol::error& e) {
            COFFEE_CORE_ERROR("[Lua Error]: {0}", e.what());
        }
    }

    void LuaBackend::RegisterFunction(std::function<void()> func, const std::string& name)
    {
        luaState.set_function(name, func);
        COFFEE_CORE_INFO("Registered Lua function {0}", name);
    }

    void LuaBackend::BindFunction(const std::string& name, std::function<void()>& func)
    {
        func = luaState[name];
        COFFEE_CORE_INFO("Bound Lua function {0}", name);
    }
} // namespace Coffee