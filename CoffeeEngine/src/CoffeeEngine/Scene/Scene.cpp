#include "Scene.h"

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/DataStructures/Octree.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Math/Frustum.h"
#include "CoffeeEngine/Physics/Collider.h"
#include "CoffeeEngine/Physics/PhysicsWorld.h"
#include "CoffeeEngine/Renderer/DebugRenderer.h"
#include "CoffeeEngine/Renderer/EditorCamera.h"
#include "CoffeeEngine/Renderer/Material.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include "CoffeeEngine/Renderer/Renderer.h"
#include "CoffeeEngine/Scene/Entity.h"
#include "CoffeeEngine/Scene/SceneCamera.h"
#include "CoffeeEngine/Scene/SceneTree.h"
#include "CoffeeEngine/Scripting/Lua/LuaScript.h"
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

    Scene::Scene() : m_Octree({glm::vec3(-50.0f), glm::vec3(50.0f)}, 10, 5)
    {
        m_SceneTree = CreateScope<SceneTree>(this);
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
        // Create floor (static box)
        // Create floor entity
        Entity floorEntity = CreateEntity("Floor");
        auto& floorTransform = floorEntity.GetComponent<TransformComponent>();
        floorTransform.Position = {0.0f, -0.25f, 0.0f};
        floorTransform.Scale = {10.0f, 0.5f, 10.0f};

        // Setup floor rigidbody
        RigidBodyConfig floorConfig;
        floorConfig.type = RigidBodyType::Static;
        floorConfig.UseGravity = false;
        floorConfig.shapeConfig.type = CollisionShapeConfig::Type::Box;
        floorConfig.shapeConfig.size = floorTransform.Scale;
        auto& floorRb = floorEntity.AddComponent<RigidbodyComponent>(floorConfig);

        // Create sphere entity
        Entity sphereEntity = CreateEntity("Sphere");
        auto& sphereTransform = sphereEntity.GetComponent<TransformComponent>();
        sphereTransform.Position = {0.0f, 5.0f, 0.0f};
        sphereTransform.Scale = {1.0f, 1.0f, 1.0f};

        // Setup sphere rigidbody
        RigidBodyConfig sphereConfig;
        sphereConfig.type = RigidBodyType::Dynamic;
        sphereConfig.UseGravity = true;
        sphereConfig.shapeConfig.type = CollisionShapeConfig::Type::Sphere;
        sphereConfig.shapeConfig.size = {0.5f, 0.5f, 0.5f};
        sphereConfig.shapeConfig.mass = 1.0f;
        auto& sphereRb = sphereEntity.AddComponent<RigidbodyComponent>(sphereConfig);

        // Create physics objects
        // Floor
        floorRb.rb->Shape = new btBoxShape(btVector3(floorTransform.Scale.x * 0.5f, floorTransform.Scale.y * 0.5f, floorTransform.Scale.z * 0.5f));
        floorRb.rb->MotionState = new btDefaultMotionState(
            btTransform(btQuaternion(0, 0, 0, 1),
            btVector3(floorTransform.Position.x, floorTransform.Position.y, floorTransform.Position.z))
        );

        btRigidBody::btRigidBodyConstructionInfo floorRbInfo(
            0.0f,
            floorRb.rb->MotionState,
            floorRb.rb->Shape
        );
        floorRb.rb->Body = new btRigidBody(floorRbInfo);

        // Sphere
        sphereRb.rb->Shape = new btSphereShape(0.5f);
        sphereRb.rb->MotionState = new btDefaultMotionState(
            btTransform(btQuaternion(0, 0, 0, 1),
            btVector3(sphereTransform.Position.x, sphereTransform.Position.y, sphereTransform.Position.z))
        );

        btRigidBody::btRigidBodyConstructionInfo sphereRbInfo(
            1.0f,
            sphereRb.rb->MotionState,
            sphereRb.rb->Shape
        );
        sphereRb.rb->Body = new btRigidBody(sphereRbInfo);
        sphereRb.rb->Body->setRestitution(0.5f);  // Add some bounciness

        // Add visual meshes
        floorEntity.AddComponent<MeshComponent>(PrimitiveMesh::CreateCube());
        sphereEntity.AddComponent<MeshComponent>(PrimitiveMesh::CreateSphere());

        // Add rigidbodies to physics world
        physicsWorld.addRigidBody(floorRb.rb->Body);
        physicsWorld.addRigidBody(sphereRb.rb->Body);
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
    }

    void Scene::OnUpdateEditor(EditorCamera& camera, float dt)
    {
        ZoneScoped;

        m_SceneTree->Update();

        Renderer::GetCurrentRenderTarget()->SetCamera(camera, glm::inverse(camera.GetViewMatrix()));

        // TEST ------------------------------
        m_Octree.DebugDraw();

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
            Ref<AnimationSystem> animation = meshComponent.animator ? meshComponent.animator->m_AnimationSystem : nullptr;
            
            //Renderer::Submit(material, mesh, transformComponent.GetWorldTransform(), (uint32_t)entity);
            Renderer::Submit(RenderCommand{transformComponent.GetWorldTransform(), mesh, material, (uint32_t)entity, animation});
        }

        auto animatorView = m_Registry.view<AnimatorComponent>();

        for (auto& entity : animatorView)
        {
            auto& animatorComponent = animatorView.get<AnimatorComponent>(entity);
            auto& animatorSystem = animatorComponent.m_AnimationSystem;

            animatorSystem->Update(dt);
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

        // ------------------------------ TEMPORAL ------------------------------
        // --------------------------- Physics testing --------------------------
        physicsWorld.stepSimulation(dt);

        // Update transforms from physics
        auto viewPhysics = m_Registry.view<RigidbodyComponent, TransformComponent>();
        for (auto entity : viewPhysics)
        {
            auto [rb, transform] = viewPhysics.get<RigidbodyComponent, TransformComponent>(entity);
            if (rb.rb && rb.rb->Body && rb.rb->Body->getMotionState())
            {
                btTransform trans;
                rb.rb->Body->getMotionState()->getWorldTransform(trans);
                transform.Position = {trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z()};
            }
        }

        // Print sphere position
        auto sphereEntity = GetEntityByName("Sphere");
        if(sphereEntity)
        {
            auto& sphereTransform = sphereEntity.GetComponent<TransformComponent>();
            COFFEE_INFO("Sphere position: {0}, {1}, {2}", sphereTransform.Position.x, sphereTransform.Position.y, sphereTransform.Position.z);
        }
        // ------------------------- END Physics testing ------------------------

        // Get all entities with ScriptComponent
        auto scriptView = m_Registry.view<ScriptComponent>();

        for (auto& entity : scriptView)
        {
            auto& scriptComponent = scriptView.get<ScriptComponent>(entity);
            scriptComponent.script->OnUpdate(dt);
        }

        //TODO: Add this to a function bc it is repeated in OnUpdateEditor
        Renderer::GetCurrentRenderTarget()->SetCamera(*camera, cameraTransform);

        m_Octree.DebugDraw();

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
            
            Renderer::Submit(RenderCommand{transformComponent.GetWorldTransform(), mesh, material, (uint32_t)entity});
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

        // Get all entities with ScriptComponent
        auto scriptView = m_Registry.view<ScriptComponent>();

        for (auto& entity : scriptView)
        {
            Entity scriptEntity{entity, this};
            ScriptManager::RegisterVariable("entity", (void*)&scriptEntity);

            auto& scriptComponent = scriptView.get<ScriptComponent>(entity);

            scriptComponent.script.OnUpdate();
        }

        Renderer::EndScene();
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
        // TODO remove temporal code to reset bodies positions
        auto view = m_Registry.view<RigidbodyComponent, TransformComponent>();
        for (auto entity : view) {
            auto [rb, transform] = view.get<RigidbodyComponent, TransformComponent>(entity);
            if (rb.rb && rb.rb->Body) {
                physicsWorld.removeRigidBody(rb.rb->Body);

                // Reset transform
                Entity e{entity, this};
                if (e.GetComponent<TagComponent>().Tag == "Sphere") {
                    transform.Position = {0.0f, 5.0f, 0.0f}; // Reset to initial position
                }
                else if (e.GetComponent<TagComponent>().Tag == "Floor") {
                    transform.Position = {0.0f, -0.25f, 0.0f};
                }

                // Reset physics state
                btTransform trans;
                trans.setIdentity();
                trans.setOrigin(btVector3(transform.Position.x, transform.Position.y, transform.Position.z));

                rb.rb->Body->setWorldTransform(trans);
                rb.rb->Body->setInterpolationWorldTransform(trans);
                rb.rb->Body->setLinearVelocity(btVector3(0,0,0));
                rb.rb->Body->setAngularVelocity(btVector3(0,0,0));
                rb.rb->Body->clearForces();
            }
        }
    }

    Ref<Scene> Scene::Load(const std::filesystem::path& path)
    {
        ZoneScoped;

        Ref<Scene> scene = CreateRef<Scene>();

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
            .get<AudioSourceComponent>(archive)
            .get<AudioListenerComponent>(archive)
            .get<AudioZoneComponent>(archive);
        
        scene->m_FilePath = path;

        auto view = scene->m_Registry.view<entt::entity>();
        for (auto entity : view)
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

        //archive(*scene);

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
            .get<AudioSourceComponent>(archive)
            .get<AudioListenerComponent>(archive)
            .get<AudioZoneComponent>(archive);
        
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

        if (model->GetAnimationSystem())
            animatorComponent = &modelEntity.AddComponent<AnimatorComponent>(model->GetAnimationSystem());

        if((entt::entity)parent != entt::null)modelEntity.SetParent(parent);
        modelEntity.GetComponent<TransformComponent>().SetLocalTransform(model->GetTransform());

        auto& meshes = model->GetMeshes();
        bool hasMultipleMeshes = meshes.size() > 1;

        for(auto& mesh : meshes)
        {
            Entity entity = hasMultipleMeshes ? scene->CreateEntity(mesh->GetName()) : modelEntity;

            entity.AddComponent<MeshComponent>(mesh);

            if (animatorComponent)
                entity.GetComponent<MeshComponent>().animator = animatorComponent;

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
