#include "ScriptComponent.h"
#include "CoffeeEngine/Project/Project.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Scripting/Script.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

namespace Coffee 
{
    template <class Archive> 
    void ScriptComponent::save(Archive& archive, std::uint32_t const version) const
    {
        std::filesystem::path relativePath;
        if (Project::GetActive())
        {
            relativePath =
                std::filesystem::relative(script->GetPath(), Project::GetActive()->GetProjectDirectory());
        }
        else
        {
            relativePath = script->GetPath();
            COFFEE_CORE_ERROR("ScriptComponent::save: Project is not active, script path is not relative to the "
                              "project directory!");
        }
        archive(cereal::make_nvp("ScriptPath", relativePath.generic_string()),
                cereal::make_nvp("Language", ScriptingLanguage::Lua));
    }

    template <class Archive> 
    void ScriptComponent::load(Archive& archive, std::uint32_t const version)
    {
        std::string relativePath;
        ScriptingLanguage language;

        archive(cereal::make_nvp("ScriptPath", relativePath), cereal::make_nvp("Language", language));

        if (!relativePath.empty())
        {
            std::filesystem::path scriptPath;
            if (Project::GetActive())
            {
                scriptPath = Project::GetActive()->GetProjectDirectory() / relativePath;
            }
            else
            {
                scriptPath = relativePath;
                COFFEE_CORE_ERROR("ScriptComponent::load: Project is not active, script path is not relative to "
                                  "the project directory!");
            }

            switch (language)
            {
                using enum ScriptingLanguage;
            case Lua:
                script = ScriptManager::CreateScript(scriptPath, language);
                break;
            case cSharp:
                // Handle cSharp script loading if needed
                break;
            }
        }
    }

    // Explicit template instantiations for common cereal archives
    template void ScriptComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void ScriptComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void ScriptComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void ScriptComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
