#pragma once

#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Scripting/Lua/LuaBackend.h"
#include "CoffeeEngine/Scripting/Script.h"
#include "CoffeeEngine/Scripting/ScriptManager.h"
#include <regex>
#include <sol/sol.hpp>
#include <fstream>

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

        void ParseScript() override
        {
            std::ifstream scriptFile(m_Path);
            std::string scriptContent((std::istreambuf_iterator<char>(scriptFile)), std::istreambuf_iterator<char>());

            // TODO: The regex should be --[[export]] local variable = value

            std::regex exportRegex(R"(--\[\[export\]\]\s+(\w+)\s*=\s*(.+))"); // --[[export]] variable = value
            std::regex headerRegex(R"(--\s*\[\[header\]\]\s*(.+))"); // --[[header]] Esto es un header
            std::regex combinedRegex(R"(--\[\[export\]\]\s+(\w+)\s*=\s*(.+)|--\s*\[\[header\]\]\s*(.+))");
            std::smatch match;
            std::string::const_iterator searchStart(scriptContent.cbegin());

            while (std::regex_search(searchStart, scriptContent.cend(), match, combinedRegex)) {
                ExportedVariable variable;
                if (match[1].matched) {
                    variable.name = match[1];
                    variable.value = match[2];
                    variable.type = Sol2TypeToExportedVariableType(m_Environment[variable.name].get_type());
                } else if (match[3].matched) {
                    variable.name = "header";
                    variable.value = match[3];
                    variable.type = ExportedVariableType::None;
                }

                // Store the variable in the vector
                m_ExportedVariables[variable.name] = variable;

                // Move to the next match
                searchStart = match.suffix().first;
            }
        }

        const std::filesystem::path& GetPath() const { return m_Path; }
        const sol::environment& GetEnvironment() const { return m_Environment; }
    private:
        ExportedVariableType Sol2TypeToExportedVariableType(sol::type type)
        {
            switch (type)
            {
            case sol::type::boolean:
                return ExportedVariableType::Bool;
            case sol::type::number:
                return ExportedVariableType::Float;
            case sol::type::string:
                return ExportedVariableType::String;
            default:
                return ExportedVariableType::Int;
            }
        }
    private:
        std::filesystem::path m_Path;
        sol::environment m_Environment;
    };

}