#include "NavMeshComponent.h"
#include "CoffeeEngine/Navigation/NavMesh.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>

namespace Coffee 
{
    template <class Archive> 
    void NavMeshComponent::save(Archive& archive, std::uint32_t const version) const
    {
        archive(cereal::make_nvp("NavMesh", m_NavMesh), cereal::make_nvp("NavMeshUUID", m_NavMeshUUID));
    }

    template <class Archive> 
    void NavMeshComponent::load(Archive& archive, std::uint32_t const version)
    {
        archive(cereal::make_nvp("NavMesh", m_NavMesh), cereal::make_nvp("NavMeshUUID", m_NavMeshUUID));
    }

    // Explicit template instantiations for common cereal archives
    template void NavMeshComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void NavMeshComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void NavMeshComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void NavMeshComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
