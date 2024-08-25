#pragma once

#include "TeaEngine/Events/Event.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Scene/SceneTree.h"

#include <entt/entt.hpp>
#include <string>

namespace Tea {
    
    class Entity;
    class Model;

    class Scene
    {
    public:
        Scene();
        ~Scene() = default;

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnInit();//add diferentation between editor and game
        void OnUpdateEditor(EditorCamera& camera, float dt);
        void OnUpdateRuntime(float dt);
        void OnEvent(Event& e);
        void OnExit();
    private:
        entt::registry m_Registry;

        Scope<SceneTree> m_SceneTree;

        friend class Entity;
        friend class SceneTree;
        friend class SceneTreePanel;
    };

    //TEMP, IDK IF THIS IS WELL
    void AddModelToTheSceneTree(Scene* scene, Ref<Model> model);
}