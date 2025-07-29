#include "UIToggleComponent.h"

#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/IO/ResourceLoader.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{
    UIToggleComponent::UIToggleComponent()
    {
        OnTexture = Texture2D::Load("assets/textures/UVMap-Grid.jpg");
        OffTexture = Texture2D::Load("assets/textures/UVMap-Grid.jpg");
    }

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
        if (onTextureUUID != UUID(0))
            OnTexture = ResourceLoader::GetResource<Texture2D>(onTextureUUID);
        if (offTextureUUID != UUID(0))
            OffTexture = ResourceLoader::GetResource<Texture2D>(offTextureUUID);
        UIComponent::load(archive, version);
    }

    // Explicit template instantiations for common cereal archives
    template void UIToggleComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void UIToggleComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void UIToggleComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void UIToggleComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
