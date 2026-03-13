#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Scripting/ScriptingManager.h"
#include <cereal/cereal.hpp>
#include <filesystem>

namespace Coffee
{
    struct ScriptComponent
    {
        Ref<Script> script;

        ScriptComponent() = default;
        ScriptComponent(Ref<Script> script) : script(script) {}

        /**
         * @brief Serializes the ScriptComponent.
         *
         * This function serializes the ScriptComponent by storing the script path and language.
         * Note: Currently, this system only supports Lua scripting language.
         *
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;
        template <class Archive> void load(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::ScriptComponent, 0);
