#pragma once
#include "CoffeeEngine/Scripting/IScriptingBackend.h"

#include <filesystem>
#include <functional>
#include <sol/sol.hpp>
#include <string>

namespace Coffee {

    class LuaBackend : public IScriptingBackend {

        public:
            void Initialize() override;
            void ExecuteScript(const std::string& script) override;
            void ExecuteFile(const std::filesystem::path& filepath) override;
            void RegisterFunction(std::function<void()> func, const std::string& name) override;
            void BindFunction(const std::string& name, std::function<void()>& func) override;
        private:
            sol::state luaState;

    };

} // namespace Coffee