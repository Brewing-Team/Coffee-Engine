#pragma once
#include <entt/entity/registry.hpp>
#include "UIAnchor.h"

namespace Coffee {

    class UIManager
    {
    public:
        static void UpdateUI(entt::registry& registry);

        static AnchorPreset GetAnchorPreset(int row, int column);

    public:
        static glm::vec2 WindowSize;
    };

} // Coffee