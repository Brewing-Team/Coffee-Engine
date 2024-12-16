#pragma once

#include "CoffeeEngine/Scripting/ScriptManager.h"
#include <entt/entity/registry.hpp>
#include <sol/forward.hpp>
#include <sol/sol.hpp>

#include <functional>
#include <filesystem>

namespace Coffee
{
    class Script
    {
    public:
        Script(const std::filesystem::path& filepath, ScriptingLanguage language)
            : m_Language(language), m_Path(filepath) {}

        sol::protected_function OnCreate;
        sol::protected_function OnUpdate;
        sol::protected_function OnDestroy;

        const std::filesystem::path& GetPath() const { return m_Path; }
        const ScriptingLanguage& GetLanguage() const { return m_Language; }

    private:
        ScriptingLanguage m_Language;
        std::filesystem::path m_Path;
    };

    struct ScriptComponent
    {
        Script script;

        ScriptComponent() = default;
        ScriptComponent(const std::filesystem::path& filepath, ScriptingLanguage language, entt::registry& registry)
            : script(filepath, language)
        {
            registry.on_construct<ScriptComponent>().connect<&ScriptComponent::OnConstruct>();
            registry.on_destroy<ScriptComponent>().connect<&ScriptComponent::OnDestroy>();
        }
        ScriptComponent(const Script& script)
            : script(script) {}

        static void OnConstruct(entt::registry& registry, entt::entity entity)
        {
            auto& scriptComponent = registry.get<ScriptComponent>(entity);
            
            ScriptManager::ExecuteScriptFromFile(scriptComponent.script);
            ScriptManager::BindFunction(scriptComponent.script.GetPath().string(), "OnCreate", scriptComponent.script.OnCreate);
            ScriptManager::BindFunction(scriptComponent.script.GetPath().string(), "OnUpdate", scriptComponent.script.OnUpdate);
            ScriptManager::BindFunction(scriptComponent.script.GetPath().string(), "OnDestroy", scriptComponent.script.OnDestroy);

            scriptComponent.script.OnCreate();
        }

        static void OnDestroy(entt::registry& registry, entt::entity entity)
        {
            auto& scriptComponent = registry.get<ScriptComponent>(entity);
            scriptComponent.script.OnDestroy();
        }
    };
}