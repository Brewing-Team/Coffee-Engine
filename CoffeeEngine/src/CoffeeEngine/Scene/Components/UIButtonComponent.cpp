#include "UIButtonComponent.h"

#include "CoffeeEngine/Core/EngineContext.h"
#include "CoffeeEngine/Core/UUID.h"
#include "CoffeeEngine/Resources/ResourceManager.h"
#include "CoffeeEngine/Rendering/Texture.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{

    UIButtonComponent::UIButtonComponent() = default;

    template <class Archive> 
    void UIButtonComponent::save(Archive& archive, std::uint32_t const version) const
    {
        archive(cereal::make_nvp("Interactable", Interactable),
                cereal::make_nvp("NormalTextureUUID", NormalTexture ? NormalTexture->GetUUID() : UUID(0)),
                cereal::make_nvp("HoverTextureUUID", HoverTexture ? HoverTexture->GetUUID() : UUID(0)),
                cereal::make_nvp("PressedTextureUUID", PressedTexture ? PressedTexture->GetUUID() : UUID(0)),
                cereal::make_nvp("DisabledTextureUUID", DisabledTexture ? DisabledTexture->GetUUID() : UUID(0)),
                cereal::make_nvp("NormalColor", NormalColor), cereal::make_nvp("HoverColor", HoverColor),
                cereal::make_nvp("PressedColor", PressedColor), cereal::make_nvp("DisabledColor", DisabledColor));
        UIComponent::save(archive, version);
    }

    template <class Archive> 
    void UIButtonComponent::load(Archive& archive, std::uint32_t const version)
    {
        UUID NormalTextureUUID;
        UUID HoverTextureUUID;
        UUID PressedTextureUUID;
        UUID DisabledTextureUUID;

        archive(cereal::make_nvp("Interactable", Interactable),
                cereal::make_nvp("NormalTextureUUID", NormalTextureUUID),
                cereal::make_nvp("HoverTextureUUID", HoverTextureUUID),
                cereal::make_nvp("PressedTextureUUID", PressedTextureUUID),
                cereal::make_nvp("DisabledTextureUUID", DisabledTextureUUID),
                cereal::make_nvp("NormalColor", NormalColor), cereal::make_nvp("HoverColor", HoverColor),
                cereal::make_nvp("PressedColor", PressedColor), cereal::make_nvp("DisabledColor", DisabledColor));
        PendingNormalTextureID = NormalTextureUUID;
        PendingHoverTextureID = HoverTextureUUID;
        PendingPressedTextureID = PressedTextureUUID;
        PendingDisabledTextureID = DisabledTextureUUID;
        UIComponent::load(archive, version);
    }

    void UIButtonComponent::ResolveResources(EngineContext& context)
    {
        if (!context.resourceManager)
            return;

        if (!NormalTexture && PendingNormalTextureID != ResourceID::null)
            NormalTexture = context.resourceManager->GetResource<Texture2D>(PendingNormalTextureID);
        if (!HoverTexture && PendingHoverTextureID != ResourceID::null)
            HoverTexture = context.resourceManager->GetResource<Texture2D>(PendingHoverTextureID);
        if (!PressedTexture && PendingPressedTextureID != ResourceID::null)
            PressedTexture = context.resourceManager->GetResource<Texture2D>(PendingPressedTextureID);
        if (!DisabledTexture && PendingDisabledTextureID != ResourceID::null)
            DisabledTexture = context.resourceManager->GetResource<Texture2D>(PendingDisabledTextureID);

        PendingNormalTextureID = ResourceID::null;
        PendingHoverTextureID = ResourceID::null;
        PendingPressedTextureID = ResourceID::null;
        PendingDisabledTextureID = ResourceID::null;
    }

    // Explicit template instantiations for common cereal archives
    template void UIButtonComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void UIButtonComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void UIButtonComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void UIButtonComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
