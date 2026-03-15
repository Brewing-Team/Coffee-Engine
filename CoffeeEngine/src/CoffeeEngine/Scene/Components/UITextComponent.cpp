#include "UITextComponent.h"

#include "CoffeeEngine/Core/EngineContext.h"
#include "CoffeeEngine/Project/Project.h"
#include "CoffeeEngine/Project/ProjectManager.h"
#include "CoffeeEngine/Rendering/Font.h"

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
            cereal::make_nvp("FontPath", FontPath.generic_string()),
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

        FontPath = relativePath;
        UIComponent::load(archive, version);
    }

    void UITextComponent::ResolveResources(EngineContext& context)
    {
        if (UIFont)
            return;

        if (FontPath.empty())
        {
            UIFont = Font::GetDefault();
            return;
        }

        std::filesystem::path resolvedPath = FontPath;
        if (!resolvedPath.is_absolute() && context.projectManager)
        {
            if (const Ref<const Project> project = context.projectManager->GetCurrentProject())
                resolvedPath = project->GetDirectory() / resolvedPath;
        }

        UIFont = CreateRef<Coffee::Font>(resolvedPath);
    }

    // Explicit template instantiations for common cereal archives
    template void UITextComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void UITextComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void UITextComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void UITextComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
