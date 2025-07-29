#include "AudioZoneComponent.h"
#include "CoffeeEngine/IO/Serialization/GLMSerialization.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

namespace Coffee 
{
    template <class Archive> 
    void AudioZoneComponent::save(Archive& archive, std::uint32_t const version) const
    {
        archive(cereal::make_nvp("ZoneID", zoneID), cereal::make_nvp("AudioBusName", audioBusName),
                cereal::make_nvp("Position", position), cereal::make_nvp("Radius", radius));
    }

    template <class Archive> 
    void AudioZoneComponent::load(Archive& archive, std::uint32_t const version)
    {
        archive(cereal::make_nvp("ZoneID", zoneID), cereal::make_nvp("AudioBusName", audioBusName),
                cereal::make_nvp("Position", position), cereal::make_nvp("Radius", radius));
    }

    // Explicit template instantiations for common cereal archives
    template void AudioZoneComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void AudioZoneComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void AudioZoneComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void AudioZoneComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
