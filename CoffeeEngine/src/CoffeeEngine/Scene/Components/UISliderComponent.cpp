#include "UISliderComponent.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/IO/ResourceLoader.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{
    UISliderComponent::UISliderComponent()
    {
        BackgroundTexture = Texture2D::Load("assets/textures/UVMap-Grid.jpg");
        HandleTexture = Texture2D::Load("assets/textures/UVMap-Grid.jpg");
        DisabledHandleTexture = Texture2D::Load("assets/textures/UVMap-Grid.jpg");
        HandleScale = {1.0f, 1.0f};
    }

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

        if (BackgroundTextureUUID != UUID(0))
            BackgroundTexture = ResourceLoader::GetResource<Texture2D>(BackgroundTextureUUID);
        if (HandleTextureUUID != UUID(0))
            HandleTexture = ResourceLoader::GetResource<Texture2D>(HandleTextureUUID);

        if (version >= 1)
        {
            archive(cereal::make_nvp("DisabledHandleTextureUUID", DisabledHandleTextureUUID));

            if (DisabledHandleTextureUUID != UUID(0))
                DisabledHandleTexture = ResourceLoader::GetResource<Texture2D>(DisabledHandleTextureUUID);
        }
        UIComponent::load(archive, version);
    }

    // Explicit template instantiations for common cereal archives
    template void UISliderComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void UISliderComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void UISliderComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void UISliderComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
