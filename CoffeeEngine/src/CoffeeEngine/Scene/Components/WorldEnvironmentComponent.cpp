#include "WorldEnvironmentComponent.h"
#include "CoffeeEngine/IO/ResourceLoader.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/IO/Serialization/GLMSerialization.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{
    template <class Archive> 
    void WorldEnvironmentComponent::save(Archive& archive, std::uint32_t const version) const
    {
        UUID skyboxUUID = Skybox ? Skybox->GetUUID() : UUID::null;
        archive(cereal::make_nvp("Skybox", skyboxUUID));

        archive(cereal::make_nvp("SkyboxIntensity", SkyboxIntensity));
        archive(cereal::make_nvp("TonemappingExposure", TonemappingExposure));
        archive(cereal::make_nvp("Fog", Fog), cereal::make_nvp("FogColor", FogColor),
                cereal::make_nvp("FogDensity", FogDensity), cereal::make_nvp("FogHeight", FogHeight),
                cereal::make_nvp("FogHeightDensity", FogHeightDensity));
        archive(cereal::make_nvp("Bloom", Bloom),
                cereal::make_nvp("BloomIntensity", BloomIntensity),
                cereal::make_nvp("BloomRadius", BloomRadius),
                cereal::make_nvp("MaxMipLevels", BloomMaxMipLevels));
    }

    template <class Archive> 
    void WorldEnvironmentComponent::load(Archive& archive, std::uint32_t const version)
    {
        UUID skyboxUUID;
        archive(cereal::make_nvp("Skybox", skyboxUUID));
        if(skyboxUUID != UUID::null) this->Skybox = ResourceLoader::GetResource<Cubemap>(skyboxUUID);
        archive(cereal::make_nvp("SkyboxIntensity", SkyboxIntensity));

        if (version >= 1)
        {
            archive(cereal::make_nvp("TonemappingExposure", TonemappingExposure));

            archive(cereal::make_nvp("Fog", Fog), cereal::make_nvp("FogColor", FogColor),
                    cereal::make_nvp("FogDensity", FogDensity), cereal::make_nvp("FogHeight", FogHeight),
                    cereal::make_nvp("FogHeightDensity", FogHeightDensity));
        }

        if (version >= 2)
        {
            archive(cereal::make_nvp("Bloom", Bloom),
                    cereal::make_nvp("BloomIntensity", BloomIntensity),
                    cereal::make_nvp("BloomRadius", BloomRadius),
                    cereal::make_nvp("MaxMipLevels", BloomMaxMipLevels));
        }
    }

    // Explicit template instantiations for common cereal archives
    template void WorldEnvironmentComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void WorldEnvironmentComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void WorldEnvironmentComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void WorldEnvironmentComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
