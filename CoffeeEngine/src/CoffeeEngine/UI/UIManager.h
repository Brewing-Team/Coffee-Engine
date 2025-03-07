#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Scene/Components.h"
#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace Coffee {
    class Scene;
    class UIManager {
    public:
        UIManager() = default;
        ~UIManager() = default;

        // Editor functions
        void OnEditorUpdate(float dt, entt::registry& registry);

        // Runtime functions
        void OnRuntimeUpdate(float dt, entt::registry& registry);
    };
}
