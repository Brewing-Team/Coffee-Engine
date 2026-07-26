#pragma once

#include "UIComponent.h"
#include "CoffeeEngine/Core/Base.h"

namespace Coffee
{
    class Texture2D;
}

namespace Coffee
{
    struct UIToggleComponent : public UIComponent
    {
        UIToggleComponent();

        bool Value = false;        ///< The value of the toggle.
        Ref<Texture2D> OnTexture;  ///< The texture when the toggle is on.
        Ref<Texture2D> OffTexture; ///< The texture when the toggle is off.

        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;

        template <class Archive> void load(Archive& archive, std::uint32_t const version);
    };

}

CEREAL_CLASS_VERSION(Coffee::UIToggleComponent, 0);
