#pragma once

#include "TeaEngine/Events/Event.h"
#include "TeaEngine/Renderer/EditorCamera.h"

#include <entt/entt.hpp>
#include <string>

namespace Tea {
    
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene() = default;

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnInit();
        void OnUpdate();
        void OnUpdateEditor(EditorCamera& camera); //IS TEMPORAL, I DONT LIKE THE NAME AND I FEEL THAT ITS A DUPLICATED OF ONUPDATE
        void OnEvent(Event& e);
        void OnExit();
    private:
        entt::registry m_Registry;

        friend class Entity;
    };

}