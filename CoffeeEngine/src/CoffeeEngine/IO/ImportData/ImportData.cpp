#include "ImportData.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee {

    // ImportData implementation
    template<typename Archive>
    void ImportData::serialize(Archive& archive, std::uint32_t const version)
    {
        archive(CEREAL_NVP(uuid), CEREAL_NVP(type), CEREAL_NVP(originalPath), CEREAL_NVP(cachedPath), CEREAL_NVP(cache), CEREAL_NVP(internal));
    }

} // namespace Coffee

// Explicit template instantiations for common cereal archives
template void Coffee::ImportData::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
template void Coffee::ImportData::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const);
template void Coffee::ImportData::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
template void Coffee::ImportData::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const);
