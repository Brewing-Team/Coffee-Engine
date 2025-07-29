#include "UIButtonComponent.h"

#include "CoffeeEngine/Core/UUID.h"
#include "CoffeeEngine/IO/ResourceLoader.h"
#include "CoffeeEngine/Renderer/Texture.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{

    UIButtonComponent::UIButtonComponent()
    {
        NormalTexture = Texture2D::Load("assets/textures/UVMap-Grid.jpg");
        HoverTexture = Texture2D::Load("assets/textures/UVMap-Grid.jpg");
        PressedTexture = Texture2D::Load("assets/textures/UVMap-Grid.jpg");
        DisabledTexture = Texture2D::Load("assets/textures/UVMap-Grid.jpg");
    }

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
        if (NormalTextureUUID != UUID(0))
            NormalTexture = ResourceLoader::GetResource<Texture2D>(NormalTextureUUID);
        if (HoverTextureUUID != UUID(0))
            HoverTexture = ResourceLoader::GetResource<Texture2D>(HoverTextureUUID);
        if (PressedTextureUUID != UUID(0))
            PressedTexture = ResourceLoader::GetResource<Texture2D>(PressedTextureUUID);
        if (DisabledTextureUUID != UUID(0))
            DisabledTexture = ResourceLoader::GetResource<Texture2D>(DisabledTextureUUID);
        UIComponent::load(archive, version);
    }

    // Explicit template instantiations for common cereal archives
    template void UIButtonComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void UIButtonComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void UIButtonComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void UIButtonComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
