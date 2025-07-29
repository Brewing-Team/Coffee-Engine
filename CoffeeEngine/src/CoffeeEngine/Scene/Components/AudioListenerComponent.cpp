#include "AudioListenerComponent.h"
#include "CoffeeEngine/IO/Serialization/GLMSerialization.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{
    template <class Archive> 
    void AudioListenerComponent::save(Archive& archive, std::uint32_t const version) const
    {
        archive(cereal::make_nvp("GameObjectID", gameObjectID), cereal::make_nvp("Transform", transform));
    }

    template <class Archive> 
    void AudioListenerComponent::load(Archive& archive, std::uint32_t const version)
    {
        archive(cereal::make_nvp("GameObjectID", gameObjectID), cereal::make_nvp("Transform", transform));
    }

    // Explicit template instantiations for common cereal archives
    template void AudioListenerComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void AudioListenerComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void AudioListenerComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void AudioListenerComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
