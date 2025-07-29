#include "TagComponent.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

namespace Coffee 
{
    template<class Archive>
    void TagComponent::serialize(Archive& archive, std::uint32_t const version)
    {
        archive(cereal::make_nvp("Tag", Tag));
    }

    // Explicit template instantiations for common cereal archives
    template void TagComponent::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void TagComponent::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const);
    template void TagComponent::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
    template void TagComponent::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const);
}
