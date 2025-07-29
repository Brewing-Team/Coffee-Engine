#pragma once

#include "UIComponent.h"
#include "CoffeeEngine/Core/Base.h"
#include <cereal/cereal.hpp>

namespace Coffee
{
    class Texture2D;

}

namespace Coffee
{
    struct UIImageComponent : public UIComponent
    {
        Ref<Texture2D> Texture;                      ///< The texture of the image.
        glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};  ///< The color.
        glm::vec4 UVRect = {0.0f, 0.0f, 1.0f, 1.0f}; ///< The UV rectangle.

        UIImageComponent();

        void SetTexture(const Ref<Texture2D>& texture) { Texture = texture; }

        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;
        template <class Archive> void load(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::UIImageComponent, 1);
