#include "MeshComponent.h"
#include "AnimatorComponent.h"
#include "CoffeeEngine/Core/EngineContext.h"
#include "CoffeeEngine/Resources/ResourceManager.h"
#include "CoffeeEngine/Rendering/Mesh.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{
    template <class Archive> 
    void MeshComponent::save(Archive& archive, std::uint32_t const version) const
    {
        archive(cereal::make_nvp("Mesh", mesh->GetUUID()), cereal::make_nvp("AnimatorUUID", animatorUUID));

        if (animator && animatorUUID != 0)
            animator->animatorUUID = animatorUUID;
    }

    template <class Archive> 
    void MeshComponent::load(Archive& archive, std::uint32_t const version)
    {
        UUID meshUUID;
        archive(cereal::make_nvp("Mesh", meshUUID), cereal::make_nvp("AnimatorUUID", animatorUUID));

        pendingMeshID = meshUUID;
    }

    void MeshComponent::ResolveResources(EngineContext& context)
    {
        if (mesh || pendingMeshID == ResourceID::null || !context.resourceManager)
            return;

        mesh = context.resourceManager->GetResource<Mesh>(pendingMeshID);
        if (mesh)
            mesh->ResolveResources(*context.resourceManager);

        pendingMeshID = ResourceID::null;
    }

    // Explicit template instantiations for common cereal archives
    template void MeshComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void MeshComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void MeshComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void MeshComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
