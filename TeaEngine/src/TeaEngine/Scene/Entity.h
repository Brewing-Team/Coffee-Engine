#pragma once

#include "TeaEngine/Core/Assert.h"
#include "TeaEngine/Scene/Scene.h"
#include "TeaEngine/Scene/SceneTree.h"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include <cstdint>
#include <utility>
namespace Tea {

    class Entity
    {
    public:
        Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            TEA_CORE_ASSERT(!HasComponent<T>(), "Entty already has this component!");
            T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
            return component;
        }

        template<typename T>
        T& GetComponent()
        {
            TEA_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component!");
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator entt::entity() const { return m_EntityHandle; }
        operator uint32_t() const { return (uint32_t)m_EntityHandle; }
        bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
        bool operator!=(const Entity& other) const { return !operator==(other); }

        void SetParent(Entity entity) 
        {
           HierarchyComponent::Reparent(m_Scene->m_Registry, m_EntityHandle, entity);
        }

    private:
        entt::entity m_EntityHandle{ entt::null };
        Scene* m_Scene = nullptr;
    };

}