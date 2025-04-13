#include "SceneTree.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Scene/Components.h"
#include "CoffeeEngine/Scene/Scene.h"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include <tracy/Tracy.hpp>

namespace Coffee {

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
                    auto nextEntity = lastHierarchy->m_Next;
                    auto nextHierarchy = registry.try_get<HierarchyComponent>(nextEntity);

                    if(nextHierarchy == nullptr)
                    {
                        break;
                    }

                    lastEntity = nextEntity;
                    lastHierarchy = nextHierarchy;
                }
                if (lastEntity == entity)
                {
                    return;
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

        //Prevent making a parent into a child of one of its own children
        HierarchyComponent* h = registry.try_get<HierarchyComponent>(parent);
            while (h != nullptr)
            {
                // if one of the parents iterated through is equal to the entity being re-parented, abort
                if (h->m_Parent == entity)
                {
                    auto tag = registry.try_get<TagComponent>(h->m_Parent);
                    COFFEE_CORE_WARN("Tried to re-parent {} to its own child. This is not allowed", tag ? tag->Tag : "an entity");
                    return;
                }
                // Get next parent up the chain and keep iterating
                h = registry.try_get<HierarchyComponent>(h->m_Parent);
            }
    
        HierarchyComponent::OnDestroy(registry, entity);
    
        hierarchyComponent->m_Parent = entt::null;
        hierarchyComponent->m_Next = entt::null;
        hierarchyComponent->m_Prev = entt::null;
    
        if (parent != entt::null) {

            hierarchyComponent->m_Parent = parent;
            HierarchyComponent::OnConstruct(registry, entity);
    
            // Mark the entity and its children as dirty
            auto& transformComponent = registry.get<TransformComponent>(entity);
            transformComponent.MarkDirty();
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
        auto& registry = m_Context->m_Registry;
        auto view = registry.view<TransformComponent>();
        for (auto entity : view) {
            const auto transform = registry.get<TransformComponent>(entity);
    
            if (transform.IsDirty()) {
                UpdateTransform(entity);
            }
        }
    }

    void SceneTree::UpdateTransform(entt::entity entity)
    {
        auto& registry = m_Context->m_Registry;
    
        auto& hierarchyComponent = registry.get<HierarchyComponent>(entity);
        auto& transformComponent = registry.get<TransformComponent>(entity);
    
        // Update the world transform of the entity
        if (hierarchyComponent.m_Parent != entt::null)
        {
            auto& parentTransformComponent = registry.get<TransformComponent>(hierarchyComponent.m_Parent);
            
            transformComponent.SetWorldTransform(parentTransformComponent.GetWorldTransform());
        } 
        else
        {
            transformComponent.SetWorldTransform(glm::mat4(1.0f));
        }
    
        // Recursively update all the children
        entt::entity child = hierarchyComponent.m_First;
        while (child != entt::null) {
            UpdateTransform(child);
            child = view.get<HierarchyComponent>(child).m_Next;
        }
    }

}