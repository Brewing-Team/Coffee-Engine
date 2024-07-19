#pragma once

#include "TeaEngine/Events/Event.h"

#include <entt/entt.hpp>

namespace Tea {

    class Scene
    {
    public:
        Scene();
        ~Scene() = default;

        void OnInit();
        void OnUpdate();
        void OnEvent(Event& e);
        void OnExit();
    private:
        entt::registry m_Registry;
    };

}