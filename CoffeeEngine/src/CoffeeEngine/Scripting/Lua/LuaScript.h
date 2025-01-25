#pragma once

#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Scripting/Lua/LuaBackend.h"
#include "CoffeeEngine/Scripting/Script.h"
#include "CoffeeEngine/Scripting/ScriptManager.h"
#include <sol/sol.hpp>

namespace Coffee {

    class LuaScript : public Script
    {
    public:
        LuaScript(const std::filesystem::path& path)
        {
            m_Path = path;

            //TODO: Think if this is a good way or store it in another way is better
            const LuaBackend& backend = static_cast<const LuaBackend&>(ScriptManager::GetBackend(ScriptingLanguage::Lua));
            m_Environment = sol::environment(backend.GetLuaState(), sol::create, backend.GetLuaState().globals());
        }
        ~LuaScript() = default;

        void OnReady() override
        {
            ScriptManager::ExecuteScript(*this, ScriptingLanguage::Lua);
            const sol::protected_function& onReady = m_Environment["OnReady"];
            if (!onReady.valid()) {
                COFFEE_CORE_ERROR("Lua: OnReady function is not valid.");
                return;
            }

            const sol::protected_function_result& result = onReady();
            if (!result.valid()) {
                sol::error err = result;
                COFFEE_CORE_ERROR("Lua: {0}", err.what());
            }
        }

        void OnUpdate(float dt) override
        {
            const sol::protected_function& onUpdate = m_Environment["OnUpdate"];
            if (!onUpdate.valid()) {
                COFFEE_CORE_ERROR("Lua: OnUpdate function is not valid.");
                return;
            }

            const sol::protected_function_result& result = onUpdate(dt);
            if (!result.valid()) {
                sol::error err = result;
                COFFEE_CORE_ERROR("Lua: {0}", err.what());
            }
        }

        void OnExit() override
        {
            const sol::protected_function& onExit = m_Environment["OnExit"];
            if (!onExit.valid()) {
                COFFEE_CORE_ERROR("Lua: OnExit function is not valid.");
                return;
            }

            const sol::protected_function_result& result = onExit();
            if (!result.valid()) {
                sol::error err = result;
                COFFEE_CORE_ERROR("Lua: {0}", err.what());
            }
        }

        template<typename Ret, typename... Args>
        void RegisterFunction(const std::string& name, std::function<Ret(Args...)> function)
        {
            m_Environment[name] = function;
        }

        void CallFunction(const std::string& functionName) override
        {
            const sol::protected_function& function = m_Environment[functionName];
            if (!function.valid()) {
                COFFEE_CORE_ERROR("Lua: Function {0} is not valid.", functionName);
                return;
            }

            const sol::protected_function_result& result = function();
            if (!result.valid()) {
                sol::error err = result;
                COFFEE_CORE_ERROR("Lua: {0}", err.what());
            }
        }

        template<typename T>
        void SetVariable(const std::string& name, T value)
        {
            m_Environment[name] = value;
        }

        template <typename T>
        T GetVariable(const std::string& name)
        {
            return m_Environment[name];
        }

        const std::filesystem::path& GetPath() const { return m_Path; }
        const sol::environment& GetEnvironment() const { return m_Environment; }
    private:

        std::filesystem::path m_Path;
        sol::environment m_Environment;
    };

}