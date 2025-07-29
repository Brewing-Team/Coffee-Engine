#include "AudioSourceComponent.h"

#include "CoffeeEngine/IO/Serialization/GLMSerialization.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>

namespace Coffee 
{
    template <class Archive> 
    void AudioSourceComponent::save(Archive& archive, std::uint32_t const version) const
    {
        archive(cereal::make_nvp("GameObjectID", gameObjectID), cereal::make_nvp("AudioBank", audioBank),
                cereal::make_nvp("AudioBankName", audioBankName), cereal::make_nvp("EventName", eventName),
                cereal::make_nvp("Volume", volume), cereal::make_nvp("Mute", mute),
                cereal::make_nvp("PlayOnAwake", playOnAwake), cereal::make_nvp("Transform", transform));
    }

    template <class Archive> 
    void AudioSourceComponent::load(Archive& archive, std::uint32_t const version)
    {
        archive(cereal::make_nvp("GameObjectID", gameObjectID), cereal::make_nvp("AudioBank", audioBank),
                cereal::make_nvp("AudioBankName", audioBankName), cereal::make_nvp("EventName", eventName),
                cereal::make_nvp("Volume", volume), cereal::make_nvp("Mute", mute),
                cereal::make_nvp("PlayOnAwake", playOnAwake), cereal::make_nvp("Transform", transform));
    }

    // Explicit template instantiations for common cereal archives
    template void AudioSourceComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void AudioSourceComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void AudioSourceComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void AudioSourceComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
