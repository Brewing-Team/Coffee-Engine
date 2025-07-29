#include "CameraComponent.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{
    template<class Archive>
    void CameraComponent::serialize(Archive& archive, std::uint32_t const version)
    {
        archive(cereal::make_nvp("Camera", Camera));
    }

    // Explicit template instantiations for common cereal archives
    template void CameraComponent::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void CameraComponent::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const);
    template void CameraComponent::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
    template void CameraComponent::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const);
}
