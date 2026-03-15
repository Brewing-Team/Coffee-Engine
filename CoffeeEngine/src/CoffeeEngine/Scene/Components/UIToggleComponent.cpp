#include "UIToggleComponent.h"

#include "CoffeeEngine/Core/EngineContext.h"
#include "CoffeeEngine/Rendering/Texture.h"
#include "CoffeeEngine/Resources/ResourceManager.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{
    UIToggleComponent::UIToggleComponent() = default;

    template <class Archive> 
    void UIToggleComponent::save(Archive& archive, std::uint32_t const version) const
    {
        archive(cereal::make_nvp("Value", Value),
                cereal::make_nvp("OnTextureUUID", OnTexture ? OnTexture->GetUUID() : UUID(0)),
                cereal::make_nvp("OffTextureUUID", OffTexture ? OffTexture->GetUUID() : UUID(0)));
        UIComponent::save(archive, version);
    }

    template <class Archive> 
    void UIToggleComponent::load(Archive& archive, std::uint32_t const version)
    {
        UUID onTextureUUID;
        UUID offTextureUUID;
        archive(cereal::make_nvp("Value", Value), cereal::make_nvp("OnTextureUUID", onTextureUUID),
                cereal::make_nvp("OffTextureUUID", offTextureUUID));
        PendingOnTextureID = onTextureUUID;
        PendingOffTextureID = offTextureUUID;
        UIComponent::load(archive, version);
    }

    void UIToggleComponent::ResolveResources(EngineContext& context)
    {
        if (!context.resourceManager)
            return;

        if (!OnTexture && PendingOnTextureID != ResourceID::null)
            OnTexture = context.resourceManager->GetResource<Texture2D>(PendingOnTextureID);
        if (!OffTexture && PendingOffTextureID != ResourceID::null)
            OffTexture = context.resourceManager->GetResource<Texture2D>(PendingOffTextureID);

        PendingOnTextureID = ResourceID::null;
        PendingOffTextureID = ResourceID::null;
    }

    // Explicit template instantiations for common cereal archives
    template void UIToggleComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void UIToggleComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void UIToggleComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void UIToggleComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
