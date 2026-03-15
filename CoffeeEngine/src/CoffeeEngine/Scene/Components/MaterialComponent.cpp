#include "MaterialComponent.h"
#include "CoffeeEngine/Core/EngineContext.h"
#include "CoffeeEngine/Resources/ResourceManager.h"
#include "CoffeeEngine/Rendering/Material.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{
    template <class Archive> 
    void MaterialComponent::save(Archive& archive, std::uint32_t const version) const
    {
        if (version < 1)
        {
            archive(cereal::make_nvp("Material", material->GetUUID()));
            return;
        }

        archive(cereal::make_nvp("IsEmbedded", material->IsEmbedded()));
        if (material->IsEmbedded())
        {
            archive(cereal::make_nvp("Material", material));
        }
        else
        {
            archive(cereal::make_nvp("Type", static_cast<int>(material->GetType())));

            if (material->GetType() == ResourceType::PBRMaterial)
            {
                ResourceSaver::SaveToCache<PBRMaterial>(material->GetUUID(),
                                                        std::dynamic_pointer_cast<PBRMaterial>(material));
            }
            else if (material->GetType() == ResourceType::ShaderMaterial)
            {
                ResourceSaver::SaveToCache<ShaderMaterial>(material->GetUUID(),
                                                           std::dynamic_pointer_cast<ShaderMaterial>(material));
            }

            archive(cereal::make_nvp("MaterialUUID", material->GetUUID()));
        }
    }

    template <class Archive> 
    void MaterialComponent::load(Archive& archive, std::uint32_t const version)
    {
        if (version < 1)
        {
            UUID materialUUID;
            archive(cereal::make_nvp("Material", materialUUID));
            pendingMaterialID = materialUUID;
            pendingMaterialType = ResourceType::Material;
            return;
        }

        bool isEmbedded = false;
        archive(cereal::make_nvp("IsEmbedded", isEmbedded));

        if (isEmbedded)
        {
            archive(cereal::make_nvp("Material", material));
            this->material->SetEmbedded(isEmbedded);
        }
        else
        {
            int typeInt;
            archive(cereal::make_nvp("Type", typeInt));
            pendingMaterialType = static_cast<ResourceType>(typeInt);

            UUID materialUUID;
            archive(cereal::make_nvp("MaterialUUID", materialUUID));
            pendingMaterialID = materialUUID;
        }
    }

    void MaterialComponent::ResolveResources(EngineContext& context)
    {
        if (material || pendingMaterialID == ResourceID::null || !context.resourceManager)
            return;

        switch (pendingMaterialType)
        {
            case ResourceType::PBRMaterial:
                material = context.resourceManager->GetResource<PBRMaterial>(pendingMaterialID);
                break;
            case ResourceType::ShaderMaterial:
                material = context.resourceManager->GetResource<ShaderMaterial>(pendingMaterialID);
                break;
            case ResourceType::Material:
                material = context.resourceManager->GetResource<Material>(pendingMaterialID);
                break;
            default:
                break;
        }

        pendingMaterialID = ResourceID::null;
    }

    // Explicit template instantiations for common cereal archives
    template void MaterialComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void MaterialComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void MaterialComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void MaterialComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
