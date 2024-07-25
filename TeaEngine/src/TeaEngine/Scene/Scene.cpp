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
#include "entt/entity/fwd.hpp"
#include <cstdint>
#include <string>
#include <tracy/Tracy.hpp>

namespace Tea {

    //TEMPORAL
    static MaterialTextures mTextures;
    static Ref<Material> standardMaterial;

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
        m_Registry.destroy((entt::entity)entity);
    }

    void Scene::OnInit()
    {
        ZoneScoped;

        Entity Root = CreateEntity("Root");

        for(int i = 0; i < 5; i++)
        {
            Entity child1 = CreateEntity("Child(" + std::to_string(i) + ")");
            child1.SetParent(Root);

            for(int i = 0; i < 5; i++)
            {
                Entity child2 = CreateEntity("Child(" + std::to_string(i) + ")");
                child2.SetParent(child1);
            }
        }

        //Entity e = CreateEntity("Damaged Helmet");
        //e.AddComponent<ModelComponent>("assets/models/DamagedHelmet.glb");

        AddModelToTheSceneTree(this, CreateRef<Model>("assets/models/DamagedHelmet.glb"));

/*         for(int i = 0; i < 10; i++)
        {
            Entity e = CreateEntity("Entity(" + std::to_string(i) + ")");
        } */

        mTextures.albedo = Texture::Load("assets/textures/test2.jpg");
        standardMaterial = CreateRef<Material>(mTextures);
    }

    void Scene::OnUpdate()
    {
        ZoneScoped;
    }

    void Scene::OnUpdateEditor(EditorCamera& camera)
    {
        ZoneScoped;

        m_SceneTree->Update();

        //-------------- TEMPORAL ------------------

        //temporal, in the future use UBO's for this two values
        //standardMaterial->SetViewMatrix(camera.GetViewMatrix());
        //standardMaterial->SetProjectionMatrix(camera.GetProjection());

        //------------------------------------------

        Renderer::BeginScene(camera);

        // Get all entities with ModelComponent and TransformComponent
        auto view = m_Registry.view<MeshComponent, TransformComponent>();

        // Loop through each entity with the specified components
        for (auto entity : view)
        {
            // Get the ModelComponent and TransformComponent for the current entity
            auto& meshComponent = view.get<MeshComponent>(entity);
            auto& transformComponent = view.get<TransformComponent>(entity);

            Ref<Mesh> mesh = meshComponent.GetMesh();

            //standardMaterial->SetModelMatrix(transformComponent.GetTransform());

            //standardMaterial->Use();
            Renderer::Submit(standardMaterial->GetShader(), mesh->GetVertexArray(), transformComponent.GetTransform());
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
        Entity root = scene->CreateEntity(model->GetName());

        for(auto& mesh : model->GetMeshes())
        {
            Entity meshEntity = scene->CreateEntity(mesh->GetName());
            meshEntity.AddComponent<MeshComponent>(mesh);
            meshEntity.SetParent(root);
        }
    }

}
