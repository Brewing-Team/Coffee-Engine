#include "Scene.h"

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/DataStructures/Octree.h"
#include "CoffeeEngine/Core/Input.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Renderer/EditorCamera.h"
#include "CoffeeEngine/Renderer/Material.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Renderer/Renderer.h"
#include "CoffeeEngine/Renderer/Renderer3D.h"
#include "CoffeeEngine/Scene/Components.h"
#include "CoffeeEngine/Scene/Entity.h"
#include "CoffeeEngine/Scene/SceneCamera.h"
#include "CoffeeEngine/Scene/SceneTree.h"
#include "CoffeeEngine/Scripting/Lua/LuaScript.h"
#include "PrimitiveMesh.h"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/snapshot.hpp"

#include <cstdint>
#include <cstdlib>
#include <glm/detail/type_quat.hpp>
#include <glm/fwd.hpp>
#include <memory>
#include <string>
#include <tracy/Tracy.hpp>

#include <CoffeeEngine/Scripting/Script.h>
#include <cereal/archives/json.hpp>
#include <fstream>

namespace Coffee {

    Ref<AnimationSystem> Scene::m_AnimationSystem;

    Scene::Scene() : m_Octree({glm::vec3(-50.0f), glm::vec3(50.0f)}, 10, 5)
    {
        m_SceneTree = CreateScope<SceneTree>(this);
        m_AnimationSystem = CreateRef<AnimationSystem>();
    }

/*     Scene::Scene(Ref<Scene> other)
    {
        auto& srcRegistry = other->m_Registry;
        auto& dstRegistry = m_Registry;

        auto view = srcRegistry.view<entt::entity>();
        dstRegistry.insert(view->data(), view->data() + view->size(), view->raw(), view->raw() + view.size());
    } */

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
            curr = m_Registry.get<HierarchyComponent>(curr).m_Next;
            DestroyEntity(e);
        }

        m_Registry.destroy((entt::entity)entity);
    }

    Entity Scene::GetEntityByName(const std::string& name)
    {
        auto view = m_Registry.view<TagComponent>();

        for(auto entity : view)
        {
            auto& tag = view.get<TagComponent>(entity).Tag;
            if(tag == name)
                return Entity{entity, this};
        }

        return Entity{entt::null, this};
    }

    std::vector<Entity> Scene::GetAllEntities()
    {
        std::vector<Entity> entities;

        auto view = m_Registry.view<entt::entity>();

        for(auto entity : view)
        {
            entities.push_back(Entity{entity, this});
        }

        return entities;
    }

    void Scene::OnInitEditor()
    {
        ZoneScoped;

       /*  Entity light = CreateEntity("Directional Light");
        light.AddComponent<LightComponent>().Color = {1.0f, 0.9f, 0.85f};
        light.GetComponent<TransformComponent>().Position = {0.0f, 0.8f, -2.1f};
        
        Entity camera = CreateEntity("Camera");
        camera.AddComponent<CameraComponent>();

        Ref<Shader> missingShader = CreateRef<Shader>("MissingShader", std::string(missingShaderSource));
        missingMaterial = CreateRef<Material>("Missing Material", missingShader); //TODO: Port it to use the Material::Create

        camera.AddComponent<ScriptComponent>("assets/scripts/CameraController.lua", ScriptingLanguage::Lua, m_Registry);

        Entity scriptEntity = CreateEntity("Script");
        //scriptEntity.AddComponent<ScriptComponent>("assets/scripts/test.lua", ScriptingLanguage::Lua, m_Registry); // TODO move the registry to the ScriptManager constructor
        scriptEntity.AddComponent<MeshComponent>(PrimitiveMesh::CreateCube());
        scriptEntity.AddComponent<MaterialComponent>();

        //Entity scriptEntity2 = CreateEntity("Script2");
        //scriptEntity2.AddComponent<ScriptComponent>("assets/scripts/test2.lua", ScriptingLanguage::Lua, m_Registry); // TODO move the registry to the ScriptManager constructor*/

        // ------------------------------ TEMPORAL ------------------------------
        // --------------------------- Physics testing --------------------------
        CollisionSystem::Initialize(this);
    
        // Create floor entity & transform
        /*Entity floorEntity = CreateEntity("Floor");
        auto& floorTransform = floorEntity.GetComponent<TransformComponent>();
        floorTransform.Position = {0.0f, -0.25f, 0.0f};
        floorTransform.Scale = {10.0f, 0.5f, 10.0f};

        // Setup floor rigidbody
        RigidBody::Properties floorProps;
        floorProps.type = RigidBody::Type::Static;
        floorProps.useGravity = false;

        // Setup floor collider
        auto floorCollider = CreateRef<BoxCollider>(floorTransform.Scale);

        // Create floor rigidbody component
        auto& floorRb = floorEntity.AddComponent<RigidbodyComponent>();
        floorRb.rb = RigidBody::Create(floorProps, floorCollider);
        floorRb.rb->SetPosition(floorTransform.Position);

        // Add floor visual mesh
        // floorEntity.AddComponent<MeshComponent>(PrimitiveMesh::CreateCube());

        // Add collision callback
        floorRb.callback.OnCollisionEnter([](CollisionInfo& info) {
            COFFEE_INFO("Floor collision enter with: {}", info.entityB.GetComponent<TagComponent>().Tag);
        });
        floorRb.callback.OnCollisionStay([](CollisionInfo& info) {
            COFFEE_INFO("Floor collision stay with: {}", info.entityB.GetComponent<TagComponent>().Tag);
        });
        floorRb.callback.OnCollisionExit([](CollisionInfo& info) {
            COFFEE_INFO("Floor collision exit with: {}", info.entityB.GetComponent<TagComponent>().Tag);
        });

        // Add floor to physics world
        m_PhysicsWorld.addRigidBody(floorRb.rb->GetNativeBody());*/

        // Create spheres
        const int NUM_SPHERES = 0;
        for(int i = 0; i < NUM_SPHERES; i++) {
            Entity sphereEntity = CreateEntity("Sphere_" + std::to_string(i));

            auto& sphereTransform = sphereEntity.GetComponent<TransformComponent>();
            sphereTransform.Position = {
                static_cast<float>(rand() % 5 - 2),
                5.0f + i * 2.0f,
                static_cast<float>(rand() % 5 - 2)
            };
            sphereTransform.Scale = {1.0f, 1.0f, 1.0f};

            // Setup sphere rigidbody
            RigidBody::Properties sphereProps;
            sphereProps.type = RigidBody::Type::Dynamic;
            sphereProps.useGravity = true;
            sphereProps.mass = 1.0f;

            auto sphereCollider = CreateRef<SphereCollider>(0.5f);
            auto& sphereRb = sphereEntity.AddComponent<RigidbodyComponent>();
            sphereRb.rb = RigidBody::Create(sphereProps, sphereCollider);
            sphereRb.rb->SetPosition(sphereTransform.Position);

            // Add visual mesh and callback
            // sphereEntity.AddComponent<MeshComponent>(PrimitiveMesh::CreateSphere());

            sphereRb.callback.OnCollisionEnter([](CollisionInfo& info) {
                COFFEE_INFO("{} collision enter with: {}",
                    info.entityA.GetComponent<TagComponent>().Tag,
                    info.entityB.GetComponent<TagComponent>().Tag);
            });

            // Add to physics world
            m_PhysicsWorld.addRigidBody(sphereRb.rb->GetNativeBody());
        }
        // ------------------------- END Physics testing ------------------------

    }

    void Scene::OnInitRuntime()
    {
        ZoneScoped;

        m_SceneTree->Update();

        auto view = m_Registry.view<MeshComponent>();

        for (auto& entity : view)
        {
            auto& meshComponent = view.get<MeshComponent>(entity);
            auto& transformComponent = m_Registry.get<TransformComponent>(entity);

            ObjectContainer<Ref<Mesh>> objectContainer = {transformComponent.GetWorldTransform(), meshComponent.GetMesh()->GetAABB(), meshComponent.GetMesh()};

            m_Octree.Insert(objectContainer);
        }

        Audio::StopAllEvents();
        Audio::PlayInitialAudios();
      
        // Get all entities with ScriptComponent
        auto scriptView = m_Registry.view<ScriptComponent>();

        for (auto& entity : scriptView)
        {
            Entity scriptEntity{entity, this};

            auto& scriptComponent = scriptView.get<ScriptComponent>(entity);

            std::dynamic_pointer_cast<LuaScript>(scriptComponent.script)->SetVariable("self", scriptEntity);
            std::dynamic_pointer_cast<LuaScript>(scriptComponent.script)->SetVariable("current_scene", this);

            scriptComponent.script->OnReady();
        }
    }

    void Scene::OnUpdateEditor(EditorCamera& camera, float dt)
    {
        ZoneScoped;

        m_SceneTree->Update();

        Renderer::GetCurrentRenderTarget()->SetCamera(camera, glm::inverse(camera.GetViewMatrix()));

        // TEST ------------------------------
        m_Octree.DebugDraw();

        auto animatorView = m_Registry.view<AnimatorComponent>();

        for (auto& entity : animatorView)
        {
            AnimatorComponent* animatorComponent = &animatorView.get<AnimatorComponent>(entity);
            animatorComponent->GetAnimationSystem()->Update(dt, animatorComponent);
        }

        UpdateAudioComponentsPositions();

        // Get all entities with ModelComponent and TransformComponent
        auto view = m_Registry.view<MeshComponent, TransformComponent>();

        // Loop through each entity with the specified components
        for (auto& entity : view)
        {
            // Get the ModelComponent and TransformComponent for the current entity
            auto& meshComponent = view.get<MeshComponent>(entity);
            auto& transformComponent = view.get<TransformComponent>(entity);
            auto materialComponent = m_Registry.try_get<MaterialComponent>(entity);

            Ref<Mesh> mesh = meshComponent.GetMesh();
            Ref<Material> material = (materialComponent) ? materialComponent->material : nullptr;

            //Renderer::Submit(material, mesh, transformComponent.GetWorldTransform(), (uint32_t)entity);
            Renderer3D::Submit(RenderCommand{transformComponent.GetWorldTransform(), mesh, material, (uint32_t)entity, meshComponent.animator});
        }

        //Get all entities with LightComponent and TransformComponent
        auto lightView = m_Registry.view<LightComponent, TransformComponent>();

        //Loop through each entity with the specified components
        for(auto& entity : lightView)
        {
            auto& lightComponent = lightView.get<LightComponent>(entity);
            auto& transformComponent = lightView.get<TransformComponent>(entity);

            lightComponent.Position = transformComponent.GetWorldTransform()[3];
            lightComponent.Direction = glm::normalize(glm::vec3(-transformComponent.GetWorldTransform()[1]));

            Renderer3D::Submit(lightComponent);
        }
    }

    void Scene::OnUpdateRuntime(float dt)
    {
        ZoneScoped;

        m_SceneTree->Update();

        Camera* camera = nullptr;
        glm::mat4 cameraTransform;
        auto cameraView = m_Registry.view<TransformComponent, CameraComponent>();
        for(auto entity : cameraView)
        {
            auto [transform, cameraComponent] = cameraView.get<TransformComponent, CameraComponent>(entity);

            //TODO: Multiple cameras support (for now, the last camera found will be used)
            camera = &cameraComponent.Camera;
            cameraTransform = transform.GetWorldTransform();
        }

        if(!camera)
        {
            COFFEE_ERROR("No camera entity found!");

            SceneCamera sceneCamera;
            camera = &sceneCamera;

            cameraTransform = glm::mat4(1.0f);
        }

        UpdateAudioComponentsPositions();
        // Get all entities with ScriptComponent
        auto scriptView = m_Registry.view<ScriptComponent>();

        for (auto& entity : scriptView)
        {
            auto& scriptComponent = scriptView.get<ScriptComponent>(entity);
            scriptComponent.script->OnUpdate(dt);
        }

        //TODO: Add this to a function bc it is repeated in OnUpdateEditor
        Renderer::GetCurrentRenderTarget()->SetCamera(*camera, cameraTransform);

        //m_Octree.DebugDraw();

        // Get all the static meshes from the Octree
/* 
        glm::mat4 testProjection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f);

        Frustum frustum = Frustum(camera->GetProjection() * glm::inverse(cameraTransform));
        DebugRenderer::DrawFrustum(frustum, glm::vec4(1.0f), 1.0f);

        auto meshes = m_Octree.Query(frustum);

        for(auto& mesh : meshes)
        {
            Renderer::Submit(RenderCommand{mesh.transform, mesh.object, mesh.object->GetMaterial(), 0});
        } */

        auto animatorView = m_Registry.view<AnimatorComponent>();

        for (auto& entity : animatorView)
        {
            AnimatorComponent* animatorComponent = &animatorView.get<AnimatorComponent>(entity);
            animatorComponent->GetAnimationSystem()->Update(dt, animatorComponent);
        }
        
        // Get all entities with ModelComponent and TransformComponent
        auto view = m_Registry.view<MeshComponent, TransformComponent>();

        // Loop through each entity with the specified components
        for (auto& entity : view)
        {
            // Get the ModelComponent and TransformComponent for the current entity
            auto& meshComponent = view.get<MeshComponent>(entity);
            auto& transformComponent = view.get<TransformComponent>(entity);
            auto materialComponent = m_Registry.try_get<MaterialComponent>(entity);

            Ref<Mesh> mesh = meshComponent.GetMesh();
            Ref<Material> material = (materialComponent) ? materialComponent->material : nullptr;
            
            Renderer3D::Submit(RenderCommand{transformComponent.GetWorldTransform(), mesh, material, (uint32_t)entity, meshComponent.animator});
        }

        //Get all entities with LightComponent and TransformComponent
        auto lightView = m_Registry.view<LightComponent, TransformComponent>();

        //Loop through each entity with the specified components
        for(auto& entity : lightView)
        {
            auto& lightComponent = lightView.get<LightComponent>(entity);
            auto& transformComponent = lightView.get<TransformComponent>(entity);

            lightComponent.Position = transformComponent.GetWorldTransform()[3];
            lightComponent.Direction = glm::normalize(glm::vec3(-transformComponent.GetWorldTransform()[1]));

            Renderer3D::Submit(lightComponent);
        }
    }

    void Scene::OnEvent(Event& e)
    {
        ZoneScoped;
    }

    void Scene::OnExitEditor()
    {
        ZoneScoped;
    }

    void Scene::OnExitRuntime()
    {
        Audio::StopAllEvents();
    }

        Ref<Scene> Scene::Load(const std::filesystem::path& path)
    {
        ZoneScoped;
    
        Ref<Scene> scene = CreateRef<Scene>();
        
        // Initialize physics system
        CollisionSystem::Initialize(scene.get());
    
        std::ifstream sceneFile(path);
        cereal::JSONInputArchive archive(sceneFile);
    
        entt::snapshot_loader{scene->m_Registry}
            .get<entt::entity>(archive)
            .get<TagComponent>(archive)
            .get<TransformComponent>(archive)
            .get<HierarchyComponent>(archive)
            .get<CameraComponent>(archive)
            .get<MeshComponent>(archive)
            .get<MaterialComponent>(archive)
            .get<LightComponent>(archive)
            .get<ScriptComponent>(archive)
            .get<AnimatorComponent>(archive)
            .get<AudioSourceComponent>(archive)
            .get<AudioListenerComponent>(archive)
            .get<AudioZoneComponent>(archive)
            .get<LightComponent>(archive);

            scene->AssignAnimatorsToMeshes(m_AnimationSystem->GetAnimators());
        
        scene->m_FilePath = path;
    
        // Add rigidbodies back to physics world
        auto view = scene->m_Registry.view<RigidbodyComponent, TransformComponent>();
        for (auto entity : view)
        {
            auto [rb, transform] = view.get<RigidbodyComponent, TransformComponent>(entity);
            if (rb.rb && rb.rb->GetNativeBody())
            {
                // Set initial transform
                rb.rb->SetPosition(transform.Position);
                rb.rb->SetRotation(transform.Rotation);
                
                // Add to physics world
                scene->m_PhysicsWorld.addRigidBody(rb.rb->GetNativeBody());
                
                // Set user pointer for collision callbacks
                rb.rb->GetNativeBody()->setUserPointer(reinterpret_cast<void*>(static_cast<uintptr_t>(entity)));
            }
        }
    
        // Debug log
        auto entityView = scene->m_Registry.view<entt::entity>();
        for (auto entity : entityView)
        {
            auto& tag = scene->m_Registry.get<TagComponent>(entity);
            auto& hierarchy = scene->m_Registry.get<HierarchyComponent>(entity);
            COFFEE_INFO("Entity {0}, {1}", (uint32_t)entity, tag.Tag);
        }

        for (auto& audioSource : Audio::audioSources)
        {
            Audio::SetVolume(audioSource->gameObjectID, audioSource->mute ? 0.f : audioSource->volume);
        }

        return scene;
    }

    void Scene::Save(const std::filesystem::path& path, Ref<Scene> scene)
    {
        ZoneScoped;

        std::ofstream sceneFile(path);
        cereal::JSONOutputArchive archive(sceneFile);

        // archive(*scene);

        //TEMPORAL
        entt::snapshot{scene->m_Registry}
            .get<entt::entity>(archive)
            .get<TagComponent>(archive)
            .get<TransformComponent>(archive)
            .get<HierarchyComponent>(archive)
            .get<CameraComponent>(archive)
            .get<MeshComponent>(archive)
            .get<MaterialComponent>(archive)
            .get<LightComponent>(archive)
            .get<ScriptComponent>(archive)
            .get<AnimatorComponent>(archive)
            .get<AudioSourceComponent>(archive)
            .get<AudioListenerComponent>(archive)
            .get<AudioZoneComponent>(archive)
            .get<LightComponent>(archive);
        
        scene->m_FilePath = path;

        auto view = scene->m_Registry.view<entt::entity>();
        for (auto entity : view)
        {
            auto& tag = scene->m_Registry.get<TagComponent>(entity);
            auto& hierarchy = scene->m_Registry.get<HierarchyComponent>(entity);

            COFFEE_INFO("Entity {0}, {1}", (uint32_t)entity, tag.Tag);
        }
    }

    // Is possible that this function will be moved to the SceneTreePanel but for now it will stay here
    void AddModelToTheSceneTree(Scene* scene, Ref<Model> model, AnimatorComponent* animatorComponent)
    {
        static Entity parent;

        Entity modelEntity = scene->CreateEntity(model->GetName());

        if (model->HasAnimations())
        {
            animatorComponent = &modelEntity.AddComponent<AnimatorComponent>(model->GetSkeleton(), model->GetAnimationController(), Scene::GetAnimationSystem());
            animatorComponent->GetAnimationSystem()->SetCurrentAnimation(0, animatorComponent);
            animatorComponent->modelUUID = model->GetUUID();
            animatorComponent->animatorUUID = UUID();
        }

        if((entt::entity)parent != entt::null)modelEntity.SetParent(parent);
        modelEntity.GetComponent<TransformComponent>().SetLocalTransform(model->GetTransform());

        auto& meshes = model->GetMeshes();
        bool hasMultipleMeshes = meshes.size() > 1;

        for(auto& mesh : meshes)
        {
            Entity entity = hasMultipleMeshes ? scene->CreateEntity(mesh->GetName()) : modelEntity;

            entity.AddComponent<MeshComponent>(mesh);

            if (animatorComponent)
            {
                entity.GetComponent<MeshComponent>().animator = animatorComponent;
                entity.GetComponent<MeshComponent>().animatorUUID = animatorComponent->animatorUUID;
            }

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
            AddModelToTheSceneTree(scene, c, animatorComponent);
        }
    }

    void Scene::AssignAnimatorsToMeshes(const std::vector<AnimatorComponent*> animators)
    {
        std::vector<Entity> entities = GetAllEntities();
        for (auto entity : entities)
        {
            if (entity.HasComponent<MeshComponent>())
            {
                for (auto animator : animators)
                {
                    MeshComponent* meshComponent = &entity.GetComponent<MeshComponent>();
                    if (meshComponent->animatorUUID == animator->animatorUUID && !meshComponent->animator)
                        meshComponent->animator = animator;
                }
            }
        }
    }
    void Scene::UpdateAudioComponentsPositions()
    {
        auto audioSourceView = m_Registry.view<AudioSourceComponent, TransformComponent>();

        for (auto& entity : audioSourceView)
        {
            auto& audioSourceComponent = audioSourceView.get<AudioSourceComponent>(entity);
            auto& transformComponent = audioSourceView.get<TransformComponent>(entity);

            if (audioSourceComponent.transform != transformComponent.GetWorldTransform())
            {
                audioSourceComponent.transform = transformComponent.GetWorldTransform();

                Audio::Set3DPosition(audioSourceComponent.gameObjectID,
                transformComponent.GetWorldTransform()[3],
                glm::normalize(glm::vec3(transformComponent.GetWorldTransform()[2])),
                glm::normalize(glm::vec3(transformComponent.GetWorldTransform()[1]))
                );
                AudioZone::UpdateObjectPosition(audioSourceComponent.gameObjectID, transformComponent.GetWorldTransform()[3]);
            }
        }

        auto audioListenerView = m_Registry.view<AudioListenerComponent, TransformComponent>();

        for (auto& entity : audioListenerView)
        {
            auto& audioListenerComponent = audioListenerView.get<AudioListenerComponent>(entity);
            auto& transformComponent = audioListenerView.get<TransformComponent>(entity);

            if (audioListenerComponent.transform != transformComponent.GetWorldTransform())
            {
                audioListenerComponent.transform = transformComponent.GetWorldTransform();

                Audio::Set3DPosition(audioListenerComponent.gameObjectID,
                    transformComponent.GetWorldTransform()[3],
                    glm::normalize(glm::vec3(transformComponent.GetWorldTransform()[2])),
                    glm::normalize(glm::vec3(transformComponent.GetWorldTransform()[1]))
                );
            }
        }
    }

}
