#pragma once

#include "UIComponent.h"
#include "CoffeeEngine/Core/Base.h"

namespace Coffee
{
    class Texture2D;
    struct EngineContext;
}

namespace Coffee
{
    struct UIToggleComponent : public UIComponent
    {
        UIToggleComponent();

        bool Value = false;        ///< The value of the toggle.
        ResourceRef<Texture2D> OnTexture;  ///< The texture when the toggle is on.
        ResourceRef<Texture2D> OffTexture; ///< The texture when the toggle is off.

        ResourceID PendingOnTextureID = ResourceID::null;
        ResourceID PendingOffTextureID = ResourceID::null;

        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;

        template <class Archive> void load(Archive& archive, std::uint32_t const version);

        void ResolveResources(EngineContext& context);
    };

}

CEREAL_CLASS_VERSION(Coffee::UIToggleComponent, 0);
