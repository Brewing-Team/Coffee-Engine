#include "SceneTree.h"
#include "TeaEngine/Core/Log.h"
#include "TeaEngine/Scene/Scene.h"
#include "entt/entity/entity.hpp"

namespace Tea {

    HierarchyComponent::HierarchyComponent(entt::entity parent)
    {
        m_Parent = parent;
        m_First = entt::null;
        m_Next = entt::null;
        m_Prev = entt::null;
    }
    HierarchyComponent::HierarchyComponent()
    {
        m_Parent = entt::null;
        m_First = entt::null;
        m_Next = entt::null;
        m_Prev = entt::null;
    }

    void HierarchyComponent::OnConstruct(entt::registry& registry, entt::entity entity)
    {
        TEA_CORE_INFO("HierarchyComponent::OnConstruct()");

        auto& hierarchy = registry.get<HierarchyComponent>(entity);

        if(hierarchy.m_Parent != entt::null)
        {
            auto& parentHierarchy = registry.get<HierarchyComponent>(hierarchy.m_Parent);

            if(parentHierarchy.m_First == entt::null)
            {
                parentHierarchy.m_First = entity;
            }
            else
            {
                //Get the last child of the parent
                auto lastEntity = parentHierarchy.m_First;
                auto lastHierarchy = registry.try_get<HierarchyComponent>(lastEntity);
                while(lastHierarchy != nullptr && lastHierarchy->m_Next != entt::null)
                {
                    lastEntity = lastHierarchy->m_Next;
                    lastHierarchy = registry.try_get<HierarchyComponent>(lastEntity);
                }
                lastHierarchy->m_Next = entity;
                lastHierarchy->m_Prev = lastEntity;
            }
        }
    }

    void HierarchyComponent::OnDestroy(entt::registry& registry, entt::entity entity)
    {
        TEA_CORE_INFO("HierarchyComponent::OnDestroy()");
    }
    void HierarchyComponent::OnUpdate(entt::registry& registry, entt::entity entity)
    {
        TEA_CORE_INFO("HierarchyComponent::OnUpdate()");
    }

    SceneTree::SceneTree(Scene* scene) : m_Context(scene)
    {
        auto& registry = m_Context->m_Registry;
        registry.on_construct<HierarchyComponent>().connect<&HierarchyComponent::OnConstruct>();
        registry.on_update<HierarchyComponent>().connect<&HierarchyComponent::OnUpdate>();
        registry.on_destroy<HierarchyComponent>().connect<&HierarchyComponent::OnDestroy>();
    }

    void SceneTree::Update()
    {
/*         auto& registry = m_Context->m_Registry;
        auto view = registry.view<HierarchyComponent>();
        for(auto entity : view)
        {
            const auto hierarchy = registry.try_get<HierarchyComponent>(entity);
        } */
    }

}