#include "Prefab.h"

#include "SceneManager.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/IO/ResourceRegistry.h"

namespace Coffee {

    Prefab::Prefab()
        : Resource(ResourceType::Prefab)
    {
    }

    Ref<Prefab> Prefab::Create(Entity sourceEntity)
    {
        Ref<Prefab> prefab = CreateRef<Prefab>();
        prefab->SetUUID(UUID());
        prefab->SetName(sourceEntity.GetComponent<TagComponent>().Tag + "_Prefab");

        prefab->m_RootEntity = prefab->CopyEntityToPrefab(sourceEntity);

        return prefab;
    }

    entt::entity Prefab::CopyEntityToPrefab(Entity sourceEntity, entt::entity parentEntity)
    {
        entt::entity destEntity = m_Registry.create();

        // Copy standard components with their values
        CopyComponentToPrefab<TagComponent>(sourceEntity, destEntity);
        CopyComponentToPrefab<TransformComponent>(sourceEntity, destEntity);
        
        // Create hierarchy component with parent
        auto& destHierarchy = m_Registry.emplace<HierarchyComponent>(destEntity);
        destHierarchy.m_Parent = parentEntity;
        
        // Copy other standard components
        CopyComponentToPrefab<MeshComponent>(sourceEntity, destEntity);
        CopyComponentToPrefab<MaterialComponent>(sourceEntity, destEntity);
        CopyComponentToPrefab<LightComponent>(sourceEntity, destEntity);
        CopyComponentToPrefab<RigidbodyComponent>(sourceEntity, destEntity);
        CopyComponentToPrefab<ParticlesSystemComponent>(sourceEntity, destEntity);
        CopyComponentToPrefab<ScriptComponent>(sourceEntity, destEntity);
        CopyComponentToPrefab<AnimatorComponent>(sourceEntity, destEntity);
        CopyComponentToPrefab<AudioSourceComponent>(sourceEntity, destEntity);
        CopyComponentToPrefab<AudioListenerComponent>(sourceEntity, destEntity);
        CopyComponentToPrefab<AudioZoneComponent>(sourceEntity, destEntity);
        CopyComponentToPrefab<NavMeshComponent>(sourceEntity, destEntity);
        CopyComponentToPrefab<NavigationAgentComponent>(sourceEntity, destEntity);
        CopyComponentToPrefab<SpriteComponent>(sourceEntity, destEntity);
        
        // Copy empty components (which don't need values)
        CopyEmptyComponentToPrefab<StaticComponent>(sourceEntity, destEntity);
        CopyEmptyComponentToPrefab<ActiveComponent>(sourceEntity, destEntity);
        
        // Process children - this part remains unchanged
        std::vector<Entity> children = sourceEntity.GetChildren();
        for (auto& child : children)
        {
            entt::entity childEntity = CopyEntityToPrefab(child, destEntity);

            if (destHierarchy.m_First == entt::null)
            {
                destHierarchy.m_First = childEntity;
            }
            else
            {
                entt::entity lastSibling = destHierarchy.m_First;
                auto& lastSiblingHierarchy = m_Registry.get<HierarchyComponent>(lastSibling);

                while (lastSiblingHierarchy.m_Next != entt::null)
                {
                    lastSibling = lastSiblingHierarchy.m_Next;
                    lastSiblingHierarchy = m_Registry.get<HierarchyComponent>(lastSibling);
                }

                lastSiblingHierarchy.m_Next = childEntity;
                m_Registry.get<HierarchyComponent>(childEntity).m_Prev = lastSibling;
            }
        }

        return destEntity;
    }

    Entity Prefab::Instantiate(Scene* scene, const glm::mat4& transform)
    {
        if (m_RootEntity == entt::null)
        {
            COFFEE_CORE_ERROR("Prefab::Instantiate: Failed to instantiate prefab - no root entity");
            return Entity(entt::null, scene);
        }

        Scene::s_AnimatorComponents.clear();
        m_EntityMap.clear();

        Entity instance = CopyEntityToScene(scene, m_RootEntity, Entity());

        if (transform != glm::mat4(1.0f))
        {
            auto& instanceTransform = instance.GetComponent<TransformComponent>();
            instanceTransform.SetLocalTransform(transform * instanceTransform.GetLocalTransform());
        }

        if (!Scene::s_AnimatorComponents.empty())
        {
            scene->AssignAnimatorsToMeshes(Scene::s_AnimatorComponents);
        }

        return instance;
    }

    Entity Prefab::CopyEntityToScene(Scene* scene, const entt::entity prefabEntity, const Entity parent)
    {
        Entity entity = scene->CreateEntity();

        // Handle tag and transform (these are already on the entity)
        if (m_Registry.all_of<TagComponent>(prefabEntity))
        {
            const auto& tag = m_Registry.get<TagComponent>(prefabEntity);
            entity.GetComponent<TagComponent>().Tag = tag.Tag;
        }

        if (m_Registry.all_of<TransformComponent>(prefabEntity))
        {
            const auto& transform = m_Registry.get<TransformComponent>(prefabEntity);
            entity.GetComponent<TransformComponent>() = transform;
        }

        // Handle special components with custom logic
        if (m_Registry.all_of<MeshComponent>(prefabEntity))
        {
            const auto& meshComp = m_Registry.get<MeshComponent>(prefabEntity);
            
            // Create a copy of the mesh component
            MeshComponent newMeshComp = meshComp;
            
            // Update animator UUID if needed
            if (meshComp.animatorUUID != UUID())
            {
                auto it = m_EntityMap.find(meshComp.animatorUUID);
                if (it != m_EntityMap.end())
                {
                    newMeshComp.animatorUUID = it->second;
                    newMeshComp.animator = nullptr; // Will be reassigned later
                }
            }
            
            entity.AddComponent<MeshComponent>(newMeshComp);
        }
        
        if (m_Registry.all_of<AnimatorComponent>(prefabEntity))
        {
            const auto& animatorComp = m_Registry.get<AnimatorComponent>(prefabEntity);
            
            AnimatorComponent newAnimatorComp = animatorComp;
            
            newAnimatorComp.UpperAnimation = CreateRef<AnimationLayer>(*animatorComp.UpperAnimation);
            newAnimatorComp.LowerAnimation = CreateRef<AnimationLayer>(*animatorComp.LowerAnimation);
            
            UUID oldUUID = animatorComp.animatorUUID;
            UUID newUUID = UUID();
            m_EntityMap[oldUUID] = newUUID;  // Store mapping for mesh references
            newAnimatorComp.animatorUUID = newUUID;
            
            AnimationSystem::LoadAnimator(&newAnimatorComp);
            
            const std::string rootJointName = newAnimatorComp.GetSkeleton()->GetJoints()[newAnimatorComp.UpperBodyRootJoint].name;
            AnimationSystem::SetupPartialBlending(
                newAnimatorComp.UpperAnimation->CurrentAnimation,
                newAnimatorComp.LowerAnimation->CurrentAnimation,
                rootJointName,
                &newAnimatorComp
            );
            
            entity.AddComponent<AnimatorComponent>(newAnimatorComp);
            Scene::s_AnimatorComponents.push_back(&entity.GetComponent<AnimatorComponent>());
        }
        
        // Copy standard components
        CopyComponentToScene<MaterialComponent>(scene, prefabEntity, entity);
        CopyComponentToScene<LightComponent>(scene, prefabEntity, entity);
        CopyComponentToScene<RigidbodyComponent>(scene, prefabEntity, entity);
        CopyComponentToScene<ParticlesSystemComponent>(scene, prefabEntity, entity);
        CopyComponentToScene<ScriptComponent>(scene, prefabEntity, entity);
        CopyComponentToScene<AudioSourceComponent>(scene, prefabEntity, entity);
        CopyComponentToScene<AudioListenerComponent>(scene, prefabEntity, entity);
        CopyComponentToScene<AudioZoneComponent>(scene, prefabEntity, entity);
        CopyComponentToScene<NavigationAgentComponent>(scene, prefabEntity, entity);
        CopyComponentToScene<NavMeshComponent>(scene, prefabEntity, entity);
        CopyComponentToScene<SpriteComponent>(scene, prefabEntity, entity);
        
        // Copy empty components
        CopyEmptyComponentToScene<StaticComponent>(scene, prefabEntity, entity);
        
        // We don't want to add the ActiveComponent to the prefab instance
        // as it will be added to the entity when it is added to the scene
        //
        // Because of that, we can't have a Prefab without an ActiveComponent.
        // TODO Make it possible to have a prefab without an ActiveComponent
        /*
        if (m_Registry.all_of<ActiveComponent>(prefabEntity))
        {
            entity.AddComponent<ActiveComponent>();
        }
        */

        if (parent)
        {
            entity.SetParent(parent);
        }

        // Find and process all child entities of this entity
        // This approach directly queries for all entities that have the current entity as parent
        // instead of following the sibling chain (m_First -> m_Next links).
        // This is safer than the sibling chain.
        std::vector<entt::entity> childEntities;
        m_Registry.view<HierarchyComponent>().each([&](entt::entity e, const HierarchyComponent& h) {
            if (h.m_Parent == prefabEntity)
                childEntities.push_back(e);
        });

        for (auto childEntity : childEntities) {
            CopyEntityToScene(scene, childEntity, entity);
        }

        return entity;
    }

    bool Prefab::Save(const std::filesystem::path& path)
    {
        try
        {
            std::ofstream file(path);
            cereal::JSONOutputArchive archive(file);
            archive(*this);

            SetPath(path);
            ResourceRegistry::Add(UUID(), Ref<Resource>(this));

            return true;
        }
        catch(const std::exception& e)
        {
            COFFEE_CORE_ERROR("Prefab::Save: Exception during serialization: {0}", e.what());
            return false;
        }
    }

    Ref<Prefab> Prefab::Load(const std::filesystem::path& path)
    {
        try
        {
            std::ifstream file(path);
            if (!file.is_open())
            {
                COFFEE_CORE_ERROR("Prefab::Load: Failed to open file: {0}", path.string());
                return nullptr;
            }

            Ref<Prefab> prefab = CreateRef<Prefab>();
            prefab->SetPath(path);

            cereal::JSONInputArchive archive(file);
            archive(*prefab);

            ResourceRegistry::Add(UUID(), prefab);

            return prefab;
        }
        catch(const std::exception& e)
        {
            COFFEE_CORE_ERROR("Prefab::Load: Exception during deserialization: {0}", e.what());
            return nullptr;
        }
    }
}