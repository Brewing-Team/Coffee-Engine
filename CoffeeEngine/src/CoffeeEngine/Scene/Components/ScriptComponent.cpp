#include "ScriptComponent.h"
#include "CoffeeEngine/Core/EngineContext.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Project/Project.h"
#include "CoffeeEngine/Project/ProjectManager.h"
#include "CoffeeEngine/Scripting/Script.h"
#include "CoffeeEngine/Scripting/ScriptingManager.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

namespace Coffee 
{
    ScriptComponent::ScriptComponent(Ref<Script> script) : script(script)
    {
        if (script)
            ScriptPath = script->GetPath();
    }

    template <class Archive> 
    void ScriptComponent::save(Archive& archive, std::uint32_t const version) const
    {
        const std::filesystem::path serializedPath = !ScriptPath.empty() ? ScriptPath : (script ? script->GetPath() : std::filesystem::path{});
        archive(cereal::make_nvp("ScriptPath", serializedPath.generic_string()),
                cereal::make_nvp("Language", Language));
    }

    template <class Archive> 
    void ScriptComponent::load(Archive& archive, std::uint32_t const version)
    {
        std::string relativePath;

        archive(cereal::make_nvp("ScriptPath", relativePath), cereal::make_nvp("Language", Language));

        ScriptPath = relativePath;
    }

    void ScriptComponent::ResolveResources(EngineContext& context)
    {
        if (script || ScriptPath.empty() || !context.scripting)
            return;

        std::filesystem::path resolvedPath = ScriptPath;
        if (context.projectManager)
        {
            if (const Ref<const Project> project = context.projectManager->GetCurrentProject())
            {
                if (!resolvedPath.is_absolute())
                    resolvedPath = project->GetDirectory() / resolvedPath;
            }
        }

        script = context.scripting->CreateScript(resolvedPath, Language);
    }

    // Explicit template instantiations for common cereal archives
    template void ScriptComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void ScriptComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void ScriptComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void ScriptComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
