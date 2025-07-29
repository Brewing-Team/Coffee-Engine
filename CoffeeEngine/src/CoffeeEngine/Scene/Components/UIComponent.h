#pragma once

#include "CoffeeEngine/UI/UIAnchor.h"
#include <cereal/cereal.hpp>

namespace Coffee
{
    struct UIComponent
    {
        RectAnchor Anchor; ///< The anchor of the UI component.
        int Layer = 0;     ///< The layer of the UI component.

        UIComponent();
        ~UIComponent();

        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;
        template <class Archive> void load(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::UIComponent, 0);
