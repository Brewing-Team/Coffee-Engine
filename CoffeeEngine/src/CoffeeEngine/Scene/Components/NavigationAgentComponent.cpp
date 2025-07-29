#include "NavigationAgentComponent.h"
#include "NavMeshComponent.h"
#include "CoffeeEngine/Navigation/NavMeshPathfinding.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>

namespace Coffee 
{
    std::vector<glm::vec3> NavigationAgentComponent::FindPath(const glm::vec3 start, const glm::vec3 end) const
    {
        return m_PathFinder->FindPath(start, end);
    }

    template <class Archive> 
    void NavigationAgentComponent::save(Archive& archive, std::uint32_t const version) const
    {
        archive(cereal::make_nvp("NavMeshComponent", m_NavMeshComponent));
    }

    template <class Archive> 
    void NavigationAgentComponent::load(Archive& archive, std::uint32_t const version)
    {
        archive(cereal::make_nvp("NavMeshComponent", m_NavMeshComponent));

        m_PathFinder = CreateRef<NavMeshPathfinding>(m_NavMeshComponent->GetNavMesh());
    }

    // Explicit template instantiations for common cereal archives
    template void NavigationAgentComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void NavigationAgentComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void NavigationAgentComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void NavigationAgentComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
