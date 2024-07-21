#pragma once

#include "TeaEngine/Core/Base.h"
#include "entt/entity/fwd.hpp"
#include <entt/entt.hpp>

namespace Tea {

    class Scene;

    struct HierarchyComponent
    {
        HierarchyComponent(entt::entity parent);
        HierarchyComponent();

        static void OnConstruct(entt::registry& registry, entt::entity entity);

        static void OnDestroy(entt::registry& registry, entt::entity entity);
        static void OnUpdate(entt::registry& registry, entt::entity entity);

        entt::entity m_Parent;
        entt::entity m_First;
        entt::entity m_Next;
        entt::entity m_Prev;
    };

    class SceneTree
    {
    public:
        SceneTree(Scene* scene);
        ~SceneTree() = default;

        void Update();

    private:
        Scene* m_Context;
    };

}