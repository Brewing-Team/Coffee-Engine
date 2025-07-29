#include "LightComponent.h"

#include "CoffeeEngine/IO/Serialization/GLMSerialization.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{
    template<class Archive>
    void LightComponent::serialize(Archive& archive, std::uint32_t const version)
    {
        if (version >= 2)
        {
            archive(cereal::make_nvp("Color", Color), cereal::make_nvp("Direction", Direction),
                        cereal::make_nvp("Position", Position), cereal::make_nvp("Range", Range),
                        cereal::make_nvp("Attenuation", Attenuation), cereal::make_nvp("Intensity", Intensity),
                        cereal::make_nvp("Angle", Angle), cereal::make_nvp("Type", type),
                        cereal::make_nvp("Shadow", Shadow), cereal::make_nvp("ShadowBias", ShadowBias),
                        cereal::make_nvp("ShadowMaxDistance", ShadowMaxDistance),
                        cereal::make_nvp("ConeAttenuation", ConeAttenuation));
        }
        else if (version >= 1)
        {
            archive(cereal::make_nvp("Color", Color), cereal::make_nvp("Direction", Direction),
                    cereal::make_nvp("Position", Position), cereal::make_nvp("Range", Range),
                    cereal::make_nvp("Attenuation", Attenuation), cereal::make_nvp("Intensity", Intensity),
                    cereal::make_nvp("Angle", Angle), cereal::make_nvp("Type", type),
                    cereal::make_nvp("Shadow", Shadow), cereal::make_nvp("ShadowBias", ShadowBias),
                    cereal::make_nvp("ShadowMaxDistance", ShadowMaxDistance));
        }
        else
        {
            archive(cereal::make_nvp("Color", Color), cereal::make_nvp("Direction", Direction),
                    cereal::make_nvp("Position", Position), cereal::make_nvp("Range", Range),
                    cereal::make_nvp("Attenuation", Attenuation), cereal::make_nvp("Intensity", Intensity),
                    cereal::make_nvp("Angle", Angle), cereal::make_nvp("Type", type));
        }
    }

    // Explicit template instantiations for common cereal archives
    template void LightComponent::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void LightComponent::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const);
    template void LightComponent::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
    template void LightComponent::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const);
}
