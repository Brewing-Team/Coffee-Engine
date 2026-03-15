#include "UIComponent.h"
#include "CoffeeEngine/Scene/Systems/UISystem.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{
    UIComponent::UIComponent() {}
    UIComponent::~UIComponent() {}

    template <class Archive> 
    void UIComponent::save(Archive& archive, std::uint32_t const version) const
    {
        archive(cereal::make_nvp("Anchor", Anchor), cereal::make_nvp("Layer", Layer));
    }

    template <class Archive> 
    void UIComponent::load(Archive& archive, std::uint32_t const version)
    {
        archive(cereal::make_nvp("Anchor", Anchor), cereal::make_nvp("Layer", Layer));
    }

    // Explicit template instantiations for common cereal archives
    template void UIComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void UIComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void UIComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void UIComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
