#pragma once
#include <entt/entity/registry.hpp>

namespace Coffee {

    class UIManager
    {
    public:
        static void UpdateUI(entt::registry& registry);
    };

} // Coffee