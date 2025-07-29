#pragma once

#include "UIComponent.h"
#include "CoffeeEngine/Core/Base.h"

namespace Coffee
{
    class Texture2D;
}

namespace Coffee
{
    struct UIButtonComponent : public UIComponent
    {
        UIButtonComponent();

        enum class State
        {
            Normal,
            Hover,
            Pressed,
            Disabled
        };

        State CurrentState = State::Normal; ///< The current state of the button.
        bool Interactable = true;           ///< Flag to indicate if the button is interactable.

        Ref<Texture2D> NormalTexture;   ///< The texture when the button is normal.
        Ref<Texture2D> HoverTexture;    ///< The texture when the button is hovered.
        Ref<Texture2D> PressedTexture;  ///< The texture when the button is pressed.
        Ref<Texture2D> DisabledTexture; ///< The texture when the button is disabled.

        glm::vec4 NormalColor{1.0f};   ///< The color when the button is normal.
        glm::vec4 HoverColor{1.0f};    ///< The color when the button is hovered.
        glm::vec4 PressedColor{1.0f};  ///< The color when the button is pressed.
        glm::vec4 DisabledColor{1.0f}; ///< The color when the button is disabled.

        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;

        template <class Archive> void load(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::UIButtonComponent, 0);
