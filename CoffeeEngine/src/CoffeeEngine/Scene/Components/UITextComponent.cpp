#include "UITextComponent.h"

#include "CoffeeEngine/Project/Project.h"
#include "CoffeeEngine/Renderer/Font.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

namespace Coffee 
{
    template <class Archive> 
    void UITextComponent::save(Archive& archive, std::uint32_t const version) const
    {
        archive(
            cereal::make_nvp("Text", Text),
            cereal::make_nvp(
                "FontPath",
                std::filesystem::relative(FontPath, Project::GetActive()->GetProjectDirectory()).generic_string()),
            cereal::make_nvp("Color", Color), cereal::make_nvp("Kerning", Kerning),
            cereal::make_nvp("LineSpacing", LineSpacing), cereal::make_nvp("FontSize", FontSize),
            cereal::make_nvp("Alignment", Alignment));
        UIComponent::save(archive, version);
    }

    template <class Archive> 
    void UITextComponent::load(Archive& archive, std::uint32_t const version)
    {
        std::string relativePath;
        archive(cereal::make_nvp("Text", Text), cereal::make_nvp("FontPath", relativePath),
                cereal::make_nvp("Color", Color), cereal::make_nvp("Kerning", Kerning),
                cereal::make_nvp("LineSpacing", LineSpacing), cereal::make_nvp("FontSize", FontSize),
                cereal::make_nvp("Alignment", Alignment));

        if (!relativePath.empty())
        {
            FontPath = Project::GetActive()->GetProjectDirectory() / relativePath;
            UIFont = CreateRef<Coffee::Font>(FontPath);
        }
        else
            UIFont = Font::GetDefault();
        UIComponent::load(archive, version);
    }

    // Explicit template instantiations for common cereal archives
    template void UITextComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void UITextComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void UITextComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void UITextComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
