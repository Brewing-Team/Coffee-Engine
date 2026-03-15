#include "UISliderComponent.h"
#include "CoffeeEngine/Core/EngineContext.h"
#include "CoffeeEngine/Rendering/Texture.h"
#include "CoffeeEngine/Resources/ResourceManager.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{
    UISliderComponent::UISliderComponent() { HandleScale = {1.0f, 1.0f}; }

    template <class Archive> 
    void UISliderComponent::save(Archive& archive, std::uint32_t const version) const
    {
        archive(
            cereal::make_nvp("Value", Value), cereal::make_nvp("MinValue", MinValue),
            cereal::make_nvp("MaxValue", MaxValue), cereal::make_nvp("HandleScale", HandleScale),
            cereal::make_nvp("BackgroundTextureUUID", BackgroundTexture ? BackgroundTexture->GetUUID() : UUID(0)),
            cereal::make_nvp("HandleTextureUUID", HandleTexture ? HandleTexture->GetUUID() : UUID(0)));
        if (version >= 1)
        {
            archive(cereal::make_nvp("DisabledHandleTextureUUID",
                                        DisabledHandleTexture ? DisabledHandleTexture->GetUUID() : UUID(0)));
        }
        UIComponent::save(archive, version);
    }

    template <class Archive> 
    void UISliderComponent::load(Archive& archive, std::uint32_t const version)
    {
        UUID BackgroundTextureUUID;
        UUID HandleTextureUUID;
        UUID DisabledHandleTextureUUID;

        archive(cereal::make_nvp("Value", Value), cereal::make_nvp("MinValue", MinValue),
                cereal::make_nvp("MaxValue", MaxValue), cereal::make_nvp("HandleScale", HandleScale),
                cereal::make_nvp("BackgroundTextureUUID", BackgroundTextureUUID),
                cereal::make_nvp("HandleTextureUUID", HandleTextureUUID));

        PendingBackgroundTextureID = BackgroundTextureUUID;
        PendingHandleTextureID = HandleTextureUUID;

        if (version >= 1)
        {
            archive(cereal::make_nvp("DisabledHandleTextureUUID", DisabledHandleTextureUUID));

            PendingDisabledHandleTextureID = DisabledHandleTextureUUID;
        }
        UIComponent::load(archive, version);
    }

    void UISliderComponent::ResolveResources(EngineContext& context)
    {
        if (!context.resourceManager)
            return;

        if (!BackgroundTexture && PendingBackgroundTextureID != ResourceID::null)
            BackgroundTexture = context.resourceManager->GetResource<Texture2D>(PendingBackgroundTextureID);
        if (!HandleTexture && PendingHandleTextureID != ResourceID::null)
            HandleTexture = context.resourceManager->GetResource<Texture2D>(PendingHandleTextureID);
        if (!DisabledHandleTexture && PendingDisabledHandleTextureID != ResourceID::null)
            DisabledHandleTexture = context.resourceManager->GetResource<Texture2D>(PendingDisabledHandleTextureID);

        PendingBackgroundTextureID = ResourceID::null;
        PendingHandleTextureID = ResourceID::null;
        PendingDisabledHandleTextureID = ResourceID::null;
    }

    // Explicit template instantiations for common cereal archives
    template void UISliderComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void UISliderComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void UISliderComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void UISliderComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
