#pragma once

#include "TeaEngine/Core/Base.h"
#include "entt/entity/fwd.hpp"
#include <cereal/cereal.hpp>
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

        static void Reparent(entt::registry& registry, entt::entity entity, entt::entity parent);

        entt::entity m_Parent;
        entt::entity m_First;
        entt::entity m_Next;
        entt::entity m_Prev;

        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::make_nvp("Parent", m_Parent), cereal::make_nvp("First", m_First), cereal::make_nvp("Next", m_Next), cereal::make_nvp("Prev", m_Prev));
        }
    };

    class SceneTree
    {
    public:
        SceneTree(Scene* scene);
        ~SceneTree() = default;

        void Update();

        void UpdateTransform(entt::entity entity);

    private:
        Scene* m_Context;
    };

}