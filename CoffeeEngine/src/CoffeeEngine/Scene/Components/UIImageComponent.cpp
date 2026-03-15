#include "UIImageComponent.h"
#include "CoffeeEngine/Core/EngineContext.h"
#include "CoffeeEngine/Resources/ResourceManager.h"
#include "CoffeeEngine/Rendering/Texture.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{
    UIImageComponent::UIImageComponent() = default;

    template <class Archive> 
    void UIImageComponent::save(Archive& archive, std::uint32_t const version) const
    {
        archive(cereal::make_nvp("TextureUUID", Texture ? Texture->GetUUID() : UUID(0)),
                cereal::make_nvp("Color", Color), cereal::make_nvp("UVRect", UVRect));
        UIComponent::save(archive, version);
    }

    template <class Archive> 
    void UIImageComponent::load(Archive& archive, std::uint32_t const version)
    {
        UUID textureUUID;
        archive(cereal::make_nvp("TextureUUID", textureUUID));
        if (version >= 1)
        {
            archive(cereal::make_nvp("Color", Color), cereal::make_nvp("UVRect", UVRect));
        }
        PendingTextureID = textureUUID;
        UIComponent::load(archive, version);
    }

    void UIImageComponent::ResolveResources(EngineContext& context)
    {
        if (Texture || PendingTextureID == ResourceID::null || !context.resourceManager)
            return;

        Texture = context.resourceManager->GetResource<Texture2D>(PendingTextureID);
        PendingTextureID = ResourceID::null;
    }

    // Explicit template instantiations for common cereal archives
    template void UIImageComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void UIImageComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void UIImageComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void UIImageComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
