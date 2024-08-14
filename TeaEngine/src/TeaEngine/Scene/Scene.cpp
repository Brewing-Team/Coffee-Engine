#include "Scene.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Core/Log.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Renderer/Material.h"
#include "TeaEngine/Renderer/Renderer.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include "TeaEngine/Renderer/Texture.h"
#include "TeaEngine/Scene/Components.h"
#include "TeaEngine/Scene/Entity.h"
#include "TeaEngine/Renderer/Shader.h"
#include "TeaEngine/Scene/SceneTree.h"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
#include <tracy/Tracy.hpp>

namespace Tea {

    //TEMPORAL
    static Ref<Material> missingMaterial;

    Scene::Scene()
    {
        m_SceneTree = CreateScope<SceneTree>(this);
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        ZoneScoped;

        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& nameTag = entity.AddComponent<TagComponent>();
        nameTag.Tag = name.empty() ? "Entity" : name;
        entity.AddComponent<HierarchyComponent>();
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        auto& hierarchyComponent = m_Registry.get<HierarchyComponent>(entity);
        auto curr = hierarchyComponent.m_First;

        while(curr != entt::null)
        {
            Entity e{curr, this};
            DestroyEntity(e);
            curr = m_Registry.get<HierarchyComponent>(curr).m_Next;
        }

        m_Registry.destroy((entt::entity)entity);
    }

    void Scene::OnInit()
    {
        ZoneScoped;

/*         Entity Root = CreateEntity("Root");

        for(int i = 0; i < 5; i++)
        {
            Entity child1 = CreateEntity("Child(" + std::to_string(i) + ")");
            child1.SetParent(Root);

            for(int i = 0; i < 5; i++)
            {
                Entity child2 = CreateEntity("Child(" + std::to_string(i) + ")");
                child2.SetParent(child1);
            }
        } */

        AddModelToTheSceneTree(this, CreateRef<Model>("assets/models/sceneTreeImportingTest.glb"));

        Ref<Shader> missingShader = CreateRef<Shader>("assets/shaders/MissingShader.vert", "assets/shaders/MissingShader.frag");
        missingMaterial = CreateRef<Material>(missingShader);
    }

    void Scene::OnUpdate()
    {
        ZoneScoped;
    }

    void Scene::OnUpdateEditor(EditorCamera& camera)
    {
        ZoneScoped;

        m_SceneTree->Update();

        Renderer::BeginScene(camera);

        // Get all entities with ModelComponent and TransformComponent
        auto view = m_Registry.view<MeshComponent, TransformComponent>();

        // Loop through each entity with the specified components
        for (auto entity : view)
        {
            // Get the ModelComponent and TransformComponent for the current entity
            auto& meshComponent = view.get<MeshComponent>(entity);
            auto& transformComponent = view.get<TransformComponent>(entity);
            auto materialComponent = m_Registry.try_get<MaterialComponent>(entity);

            Ref<Mesh> mesh = meshComponent.GetMesh();
            Ref<Material> material = (materialComponent and materialComponent->material) ? materialComponent->material : missingMaterial;
            
            Renderer::Submit(material, mesh, transformComponent.GetWorldTransform());
        }

        Renderer::EndScene();
    }

    void Scene::OnEvent(Event& e)
    {
        ZoneScoped;
    }

    void Scene::OnExit()
    {
        ZoneScoped;
    }

    void AddModelToTheSceneTree(Scene* scene, Ref<Model> model)
    {
        static Entity parent;

        Entity modelEntity = scene->CreateEntity(model->GetName());

        if((entt::entity)parent != entt::null)modelEntity.SetParent(parent);
        modelEntity.GetComponent<TransformComponent>().SetLocalTransform(model->GetTransform());

        auto& meshes = model->GetMeshes();
        bool hasMultipleMeshes = meshes.size() > 1;

        for(auto& mesh : meshes)
        {
            Entity entity = hasMultipleMeshes ? scene->CreateEntity(mesh->GetName()) : modelEntity;

            entity.AddComponent<MeshComponent>(mesh);

            if(mesh->GetMaterial())
            {
                entity.AddComponent<MaterialComponent>(mesh->GetMaterial());
            }

            if(hasMultipleMeshes)
            {
                entity.SetParent(modelEntity);
            }
        }

        for(auto& c : model->GetChildren())
        {
            parent = modelEntity;
            AddModelToTheSceneTree(scene, c);
        }
    }

}
