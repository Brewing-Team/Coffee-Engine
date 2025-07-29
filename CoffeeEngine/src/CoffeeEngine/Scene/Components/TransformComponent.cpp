#include "TransformComponent.h"
#include "CoffeeEngine/IO/Serialization/GLMSerialization.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{
    template<class Archive>
    void TransformComponent::serialize(Archive& archive, std::uint32_t const version)
    {
        archive(cereal::make_nvp("Position", Position), cereal::make_nvp("Rotation", Rotation),
                cereal::make_nvp("Scale", Scale));
    }

    // Explicit template instantiations for common cereal archives
    template void TransformComponent::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void TransformComponent::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const);
    template void TransformComponent::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
    template void TransformComponent::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const);
}
