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

        if (sourceEntity.HasComponent<TagComponent>())
            m_Registry.emplace<TagComponent>(destEntity, sourceEntity.GetComponent<TagComponent>());

        if (sourceEntity.HasComponent<TransformComponent>())
            m_Registry.emplace<TransformComponent>(destEntity, sourceEntity.GetComponent<TransformComponent>());

        auto& destHierarchy = m_Registry.emplace<HierarchyComponent>(destEntity);
        destHierarchy.m_Parent = parentEntity;

        if (sourceEntity.HasComponent<MeshComponent>())
            m_Registry.emplace<MeshComponent>(destEntity, sourceEntity.GetComponent<MeshComponent>());

        if (sourceEntity.HasComponent<MaterialComponent>())
            m_Registry.emplace<MaterialComponent>(destEntity, sourceEntity.GetComponent<MaterialComponent>());

        if (sourceEntity.HasComponent<LightComponent>())
            m_Registry.emplace<LightComponent>(destEntity, sourceEntity.GetComponent<LightComponent>());

        if (sourceEntity.HasComponent<RigidbodyComponent>())
            m_Registry.emplace<RigidbodyComponent>(destEntity, sourceEntity.GetComponent<RigidbodyComponent>());

        if (sourceEntity.HasComponent<ParticlesSystemComponent>())
            m_Registry.emplace<ParticlesSystemComponent>(destEntity, sourceEntity.GetComponent<ParticlesSystemComponent>());

        if (sourceEntity.HasComponent<StaticComponent>())
            m_Registry.emplace<StaticComponent>(destEntity);

        if (sourceEntity.HasComponent<ActiveComponent>())
            m_Registry.emplace<ActiveComponent>(destEntity);

        if (sourceEntity.HasComponent<ScriptComponent>())
            m_Registry.emplace<ScriptComponent>(destEntity, sourceEntity.GetComponent<ScriptComponent>());

        if (sourceEntity.HasComponent<AnimatorComponent>())
            m_Registry.emplace<AnimatorComponent>(destEntity, sourceEntity.GetComponent<AnimatorComponent>());

        if (sourceEntity.HasComponent<AudioSourceComponent>())
            m_Registry.emplace<AudioSourceComponent>(destEntity, sourceEntity.GetComponent<AudioSourceComponent>());

        if (sourceEntity.HasComponent<AudioListenerComponent>())
            m_Registry.emplace<AudioListenerComponent>(destEntity, sourceEntity.GetComponent<AudioListenerComponent>());

        if (sourceEntity.HasComponent<AudioZoneComponent>())
            m_Registry.emplace<AudioZoneComponent>(destEntity, sourceEntity.GetComponent<AudioZoneComponent>());

        if (sourceEntity.HasComponent<NavMeshComponent>())
            m_Registry.emplace<NavMeshComponent>(destEntity, sourceEntity.GetComponent<NavMeshComponent>());

        if (sourceEntity.HasComponent<NavigationAgentComponent>())
            m_Registry.emplace<NavigationAgentComponent>(destEntity, sourceEntity.GetComponent<NavigationAgentComponent>());

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

        Entity instance = CopyEntityToScene(scene, m_RootEntity, Entity());

        if (transform != glm::mat4(1.0f))
        {
            auto& instanceTransform = instance.GetComponent<TransformComponent>();
            instanceTransform.SetLocalTransform(transform * instanceTransform.GetLocalTransform());
        }

        return instance;
    }

    Entity Prefab::CopyEntityToScene(Scene* scene, const entt::entity prefabEntity, const Entity parent)
    {
        Entity entity = scene->CreateEntity();

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

        if (m_Registry.all_of<MeshComponent>(prefabEntity))
        {
            const auto& meshComp = m_Registry.get<MeshComponent>(prefabEntity);
            entity.AddComponent<MeshComponent>(meshComp);
        }

        if (m_Registry.all_of<MaterialComponent>(prefabEntity))
        {
            const auto& matComp = m_Registry.get<MaterialComponent>(prefabEntity);
            entity.AddComponent<MaterialComponent>(matComp);
        }

        if (m_Registry.all_of<LightComponent>(prefabEntity))
        {
            const auto& lightComp = m_Registry.get<LightComponent>(prefabEntity);
            entity.AddComponent<LightComponent>(lightComp);
        }

        if (m_Registry.all_of<RigidbodyComponent>(prefabEntity))
        {
            const auto& rbComp = m_Registry.get<RigidbodyComponent>(prefabEntity);
            entity.AddComponent<RigidbodyComponent>(rbComp);
        }

        if (m_Registry.all_of<ParticlesSystemComponent>(prefabEntity))
        {
            const auto& particlesComp = m_Registry.get<ParticlesSystemComponent>(prefabEntity);
            entity.AddComponent<ParticlesSystemComponent>(particlesComp);
        }

        if (m_Registry.all_of<ScriptComponent>(prefabEntity))
        {
            const auto& scriptComp = m_Registry.get<ScriptComponent>(prefabEntity);
            entity.AddComponent<ScriptComponent>(scriptComp);
        }

        if (m_Registry.all_of<AnimatorComponent>(prefabEntity))
        {
            const auto& animatorComp = m_Registry.get<AnimatorComponent>(prefabEntity);
            entity.AddComponent<AnimatorComponent>(animatorComp);
        }

        if (m_Registry.all_of<AudioSourceComponent>(prefabEntity))
        {
            const auto& audioSourceComp = m_Registry.get<AudioSourceComponent>(prefabEntity);
            entity.AddComponent<AudioSourceComponent>(audioSourceComp);
        }

        if (m_Registry.all_of<AudioListenerComponent>(prefabEntity))
        {
            const auto& audioListenerComp = m_Registry.get<AudioListenerComponent>(prefabEntity);
            entity.AddComponent<AudioListenerComponent>(audioListenerComp);
        }

        if (m_Registry.all_of<AudioZoneComponent>(prefabEntity))
        {
            const auto& audioZoneComp = m_Registry.get<AudioZoneComponent>(prefabEntity);
            entity.AddComponent<AudioZoneComponent>(audioZoneComp);
        }

        if (m_Registry.all_of<NavigationAgentComponent>(prefabEntity))
        {
            const auto& navAgentComp = m_Registry.get<NavigationAgentComponent>(prefabEntity);
            entity.AddComponent<NavigationAgentComponent>(navAgentComp);
        }

        if (m_Registry.all_of<NavMeshComponent>(prefabEntity))
        {
            const auto& navMeshComp = m_Registry.get<NavMeshComponent>(prefabEntity);
            entity.AddComponent<NavMeshComponent>(navMeshComp);
        }

        if (m_Registry.all_of<StaticComponent>(prefabEntity))
        {
            entity.AddComponent<StaticComponent>();
        }

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