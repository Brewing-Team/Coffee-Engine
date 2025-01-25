#pragma once

#include <entt/entity/registry.hpp>

#include <filesystem>
#include <functional>

namespace Coffee
{
    class Script
    {
    public:
        Script() {}
        virtual void OnReady() = 0;
        virtual void OnUpdate(float dt) = 0;
        virtual void OnExit() = 0;

        // The function parameter should be more generic
        template <typename Ret, typename... Args>
        void RegisterFunction(const std::string& name, std::function<Ret(Args...)> function);
        virtual void CallFunction(const std::string& functionName) = 0;

        // The variable parameter should be more generic
        //virtual void SetVariable(const std::string& name, std::any value) = 0;
        template <typename T>
        void SetVariable(const std::string& name, T value);

        template <typename T>
        T GetVariable(const std::string& name);
    };
}