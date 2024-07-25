#include "SceneTree.h"
#include "TeaEngine/Core/Log.h"
#include "TeaEngine/Scene/Scene.h"
#include "entt/entity/entity.hpp"
#include <tracy/Tracy.hpp>

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
        auto& hierarchy = registry.get<HierarchyComponent>(entity);
        // if is the first child
        if(hierarchy.m_Prev == entt::null || !registry.valid(hierarchy.m_Prev))
        {
            if(hierarchy.m_Parent != entt::null && registry.valid(hierarchy.m_Parent))
            {
                auto parent_hierarchy = registry.try_get<HierarchyComponent>(hierarchy.m_Parent);
                if(parent_hierarchy != nullptr)
                {
                    parent_hierarchy->m_First = hierarchy.m_Next;
                    if(hierarchy.m_Next != entt::null)
                    {
                        auto next_hierarchy = registry.try_get<HierarchyComponent>(hierarchy.m_Next);
                        if(next_hierarchy != nullptr)
                        {
                            next_hierarchy->m_Prev = entt::null;
                        }
                    }
                }
            }
        }
        else
        {
            auto prev_hierarchy = registry.try_get<HierarchyComponent>(hierarchy.m_Prev);
            if(prev_hierarchy != nullptr)
            {
                prev_hierarchy->m_Next = hierarchy.m_Next;
            }
            if(hierarchy.m_Next != entt::null)
            {
                auto next_hierarchy = registry.try_get<HierarchyComponent>(hierarchy.m_Next);
                if(next_hierarchy != nullptr)
                {
                    next_hierarchy->m_Prev = hierarchy.m_Prev;
                }
            }
        }
    }
    void HierarchyComponent::OnUpdate(entt::registry& registry, entt::entity entity)
    {
        
    }

    void HierarchyComponent::Reparent(entt::registry& registry, entt::entity entity, entt::entity parent)
    {
        ZoneScoped;
        
        auto hierarchyComponent = registry.try_get<HierarchyComponent>(entity);

        HierarchyComponent::OnDestroy(registry, entity);

        hierarchyComponent->m_Parent = entt::null;
        hierarchyComponent->m_Next = entt::null;
        hierarchyComponent->m_Prev = entt::null;

        if(parent != entt::null)
        {
            hierarchyComponent->m_Parent = parent;
            HierarchyComponent::OnConstruct(registry, entity);
        }
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