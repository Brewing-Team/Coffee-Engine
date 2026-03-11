#include "Project.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/Input.h"
#include "CoffeeEngine/IO/CacheManager.h"
#include "CoffeeEngine/IO/ResourceRegistry.h"
#include "CoffeeEngine/IO/ResourceLoader.h"
#include "CoffeeEngine/Scene/SceneManager.h"
#include "CoffeeEngine/Scripting/ScriptingManager.h"
#include "CoffeeEngine/Audio/Audio.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

#include <fstream>

namespace Coffee {

    // Project implementation
    template<class Archive>
    void Project::serialize(Archive& archive, std::uint32_t const version)
    {
        archive(cereal::make_nvp("Name", m_Name),
                cereal::make_nvp("StartScene",m_StartScenePath.string()),
                cereal::make_nvp("CacheDirectory", m_CacheDirectory));

        if (version >= 1)
        {
            archive(cereal::make_nvp("AudioDirectory", m_AudioFolderPath));
        }
        else
        {
            m_AudioFolderPath = "";
        }
    }
    
    // Explicit template instantiations for common cereal archives
    template void Project::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void Project::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const);
    template void Project::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
    template void Project::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const);

} // namespace Coffee