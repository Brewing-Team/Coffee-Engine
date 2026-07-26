#pragma once

#include "UIComponent.h"
#include "CoffeeEngine/Core/Base.h"

namespace Coffee
{
    class Texture2D;
}

namespace Coffee
{
    struct UISliderComponent : public UIComponent
    {
        UISliderComponent();

        float Value = 0.0f;                   ///< The value of the slider.
        float MinValue = 0.0f;                ///< The minimum value of the slider.
        float MaxValue = 100.0f;              ///< The maximum value of the slider.
        glm::vec2 HandleScale;                ///< The scale of the handle.
        Ref<Texture2D> BackgroundTexture;     ///< The texture of the background.
        Ref<Texture2D> HandleTexture;         ///< The texture of the handle.
        Ref<Texture2D> DisabledHandleTexture; ///< The texture of the disabled handle.
        bool Selected = false;                ///< Flag to indicate if the slider is selected.

        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;

        template <class Archive> void load(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::UISliderComponent, 1);
