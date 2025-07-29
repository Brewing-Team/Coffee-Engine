#pragma once

#include "UIComponent.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Renderer2D.h"

#include <cereal/cereal.hpp>
#include <glm/vec4.hpp>

#include <filesystem>

namespace Coffee {
    class Font;
}

namespace Coffee
{
    struct UITextComponent : public UIComponent
    {
        UITextComponent() { Text = "Text"; }

        std::string Text;                                                      ///< The text.
        Ref<Font> UIFont;                                                      ///< The font.
        std::filesystem::path FontPath;                                        ///< The font path.
        glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};                            ///< The color.
        float Kerning = 0.0f;                                                  ///< The kerning.
        float LineSpacing = 0.0f;                                              ///< The line spacing.
        float FontSize = 16.0f;                                                ///< The font size.
        Renderer2D::TextAlignment Alignment = Renderer2D::TextAlignment::Left; ///< The text alignment.

        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;

        template <class Archive> void load(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::UITextComponent, 0);
