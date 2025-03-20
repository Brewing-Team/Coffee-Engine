#include "Scene.h"

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/DataStructures/Octree.h"
#include "CoffeeEngine/Core/Input.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Math/Frustum.h"
#include "CoffeeEngine/Physics/Collider.h"
#include "CoffeeEngine/Physics/CollisionCallback.h"
#include "CoffeeEngine/Physics/CollisionSystem.h"
#include "CoffeeEngine/Physics/PhysicsWorld.h"
#include "CoffeeEngine/Renderer/EditorCamera.h"
#include "CoffeeEngine/Renderer/Material.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Renderer/Renderer.h"
#include "CoffeeEngine/Renderer/Renderer3D.h"
#include "CoffeeEngine/Scene/Components.h"
#include "CoffeeEngine/Scene/Entity.h"
#include "CoffeeEngine/Scene/SceneCamera.h"
#include "CoffeeEngine/Scene/SceneManager.h"
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


    Scene::Scene() : m_Octree({glm::vec3(-50.0f), glm::vec3(50.0f)}, 10, 5)
    {
        m_SceneTree = CreateScope<SceneTree>(this);

        AnimationSystem::ResetAnimators();
    }

    template <typename T>
    static void CopyComponentIfExists(entt::entity destinyEntity, entt::entity sourceEntity, entt::registry& registry)
    {
        if(registry.all_of<T>(sourceEntity))
        {
            auto srcComponent = registry.get<T>(sourceEntity);
            registry.emplace_or_replace<T>(destinyEntity, srcComponent);
        }
    }

    template <>
    void CopyComponentIfExists<RigidbodyComponent>(entt::entity destinyEntity, entt::entity sourceEntity, entt::registry& registry)
    {
        if(registry.all_of<RigidbodyComponent>(sourceEntity))
        {
            const auto& srcComponent = registry.get<RigidbodyComponent>(sourceEntity);

            try {
                RigidBody::Properties props = srcComponent.rb->GetProperties();

                Ref<Collider> collider;
                if (auto boxCollider = std::dynamic_pointer_cast<BoxCollider>(srcComponent.rb->GetCollider())) {
                    collider = CreateRef<BoxCollider>(boxCollider->GetSize());
                }
                else if (auto sphereCollider = std::dynamic_pointer_cast<SphereCollider>(srcComponent.rb->GetCollider())) {
                    collider = CreateRef<SphereCollider>(sphereCollider->GetRadius());
                }
                else if (auto capsuleCollider = std::dynamic_pointer_cast<CapsuleCollider>(srcComponent.rb->GetCollider())) {
                    collider = CreateRef<CapsuleCollider>(capsuleCollider->GetRadius(), capsuleCollider->GetHeight());
                }
                else {
                    collider = CreateRef<BoxCollider>(glm::vec3(1.0f, 1.0f, 1.0f));
                }

                auto& newComponent = registry.emplace<RigidbodyComponent>(destinyEntity, props, collider);

                newComponent.callback = srcComponent.callback;

                if (registry.all_of<TransformComponent>(destinyEntity)) {
                    auto& transform = registry.get<TransformComponent>(destinyEntity);
                    newComponent.rb->SetPosition(transform.Position);
                    newComponent.rb->SetRotation(transform.Rotation);
                }
            }
            catch (const std::exception& e) {
                COFFEE_CORE_ERROR("Exception copying rigidbody component: {0}", e.what());
                if (registry.all_of<RigidbodyComponent>(destinyEntity)) {
                    registry.remove<RigidbodyComponent>(destinyEntity);
                }
            }
        }
    }

    template <typename... Components>
    static void CopyEntity(entt::entity destinyEntity, entt::entity sourceEntity, entt::registry& registry)
    {
        (CopyComponentIfExists<Components>(destinyEntity, sourceEntity, registry), ...);
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

    Entity Scene::Duplicate(const Entity& parent)
    {
        Entity newEntity = CreateEntity();
        CopyEntity<ALL_COMPONENTS>(newEntity, parent, m_Registry);
        return newEntity;
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

        CollisionSystem::Initialize(this);

    }

    void Scene::OnInitRuntime()
    {
        ZoneScoped;

        m_SceneTree->Update();

        CollisionSystem::Initialize(this);

/*         auto view = m_Registry.view<MeshComponent>();

        for (auto& entity : view)
        {
            auto& meshComponent = view.get<MeshComponent>(entity);
            auto& transformComponent = m_Registry.get<TransformComponent>(entity);

            ObjectContainer<Ref<Mesh>> objectContainer = {transformComponent.GetWorldTransform(), meshComponent.GetMesh()->GetAABB(), meshComponent.GetMesh()};

            m_Octree.Insert(objectContainer);
        } */

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

        // TEMPORAL - Navigation
        auto navMeshView = m_Registry.view<NavMeshComponent>();

        for (auto& entity : navMeshView)
        {
            auto& navMeshComponent = navMeshView.get<NavMeshComponent>(entity);
            if (navMeshComponent.ShowDebug && navMeshComponent.GetNavMesh() && navMeshComponent.GetNavMesh()->IsCalculated())
            {
                navMeshComponent.GetNavMesh()->RenderWalkableAreas();
            }
        }

        auto viewRigidbody = m_Registry.view<RigidbodyComponent, TransformComponent>();

        for (auto entity : viewRigidbody) {
            auto [rb, transform] = viewRigidbody.get<RigidbodyComponent, TransformComponent>(entity);
            if (rb.rb) {
                rb.rb->SetPosition(transform.Position);
                rb.rb->SetRotation(transform.Rotation);
            }
        }

        auto animatorView = m_Registry.view<AnimatorComponent>();

        for (auto& entity : animatorView)
        {
            AnimatorComponent* animatorComponent = &animatorView.get<AnimatorComponent>(entity);
            AnimationSystem::Update(dt, animatorComponent);
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


        // Get all entities with ParticlesSystemComponent and TransformComponent
        auto particleSystemView = m_Registry.view<ParticlesSystemComponent, TransformComponent>();
        for (auto& entity : particleSystemView)
        {
            auto& particlesSystemComponent = particleSystemView.get<ParticlesSystemComponent>(entity);
            auto& transformComponent = particleSystemView.get<TransformComponent>(entity);

            auto materialComponent = m_Registry.try_get<MaterialComponent>(entity);
            Ref<Material> material = (materialComponent) ? materialComponent->material : nullptr;

            if (!particlesSystemComponent.GetParticleEmitter()->particleMaterial && material)
            {
                particlesSystemComponent.GetParticleEmitter()->particleMaterial = material;
            }

            particlesSystemComponent.GetParticleEmitter()->transformComponentMatrix = transformComponent.GetWorldTransform();
            particlesSystemComponent.GetParticleEmitter()->cameraViewMatrix = camera.GetViewMatrix();
            particlesSystemComponent.GetParticleEmitter()->Update(dt);
            particlesSystemComponent.GetParticleEmitter()->DrawDebug();
        }


        // Get all entities with LightComponent and TransformComponent
        auto lightView = m_Registry.view<LightComponent, TransformComponent>();

        // Loop through each entity with the specified components
        for (auto& entity : lightView) {
            auto& lightComponent = lightView.get<LightComponent>(entity);
            auto& transformComponent = lightView.get<TransformComponent>(entity);

            lightComponent.Position = transformComponent.GetWorldTransform()[3];
            lightComponent.Direction = glm::normalize(glm::vec3(-transformComponent.GetWorldTransform()[1]));

            Renderer3D::Submit(lightComponent);
        }

        m_PhysicsWorld.drawCollisionShapes();

        OnEditorUpdateUI(dt, m_Registry);
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

        auto navMeshView = m_Registry.view<NavMeshComponent>();

        for (auto& entity : navMeshView)
        {
            auto& navMeshComponent = navMeshView.get<NavMeshComponent>(entity);
            if (navMeshComponent.ShowDebug && navMeshComponent.GetNavMesh() && navMeshComponent.GetNavMesh()->IsCalculated())
            {
                navMeshComponent.GetNavMesh()->RenderWalkableAreas();
            }
        }

        auto navigationAgentView = m_Registry.view<NavigationAgentComponent>();

        for (auto& agent : navigationAgentView)
        {
            auto& navAgentComponent = navigationAgentView.get<NavigationAgentComponent>(agent);
            if (navAgentComponent.ShowDebug && navAgentComponent.GetPathFinder())
                navAgentComponent.GetPathFinder()->RenderPath(navAgentComponent.Path);
        }

        m_PhysicsWorld.stepSimulation(dt);

        // Update transforms from physics
        auto viewPhysics = m_Registry.view<RigidbodyComponent, TransformComponent>();
        for (auto entity : viewPhysics) {
            auto [rb, transform] = viewPhysics.get<RigidbodyComponent, TransformComponent>(entity);
            if (rb.rb) {
                transform.Position = rb.rb->GetPosition();
                transform.Rotation = rb.rb->GetRotation();
            }
        }

        UpdateAudioComponentsPositions();

        // Get all entities with ScriptComponent
        auto scriptView = m_Registry.view<ScriptComponent>();

        for (auto& entity : scriptView)
        {
            auto& scriptComponent = scriptView.get<ScriptComponent>(entity);
            scriptComponent.script->OnUpdate(dt);
            if(SceneManager::GetActiveScene().get() != this)
                return;
        }

        if(SceneManager::GetActiveScene().get() != this)
            return;

        //TODO: Add this to a function bc it is repeated in OnUpdateEditor
        Renderer::GetCurrentRenderTarget()->SetCamera(*camera, cameraTransform);

        //m_Octree.DebugDraw();

        // Get all the static meshes from the Octree
/* 
        glm::mat4 testProjection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f);

        Frustum frustum = Frustum(camera->GetProjection() * glm::inverse(cameraTransform));
        Renderer2D::DrawFrustum(frustum, glm::vec4(1.0f), 1.0f);

        auto meshes = m_Octree.Query(frustum);

        for(auto& mesh : meshes)
        {
            Renderer::Submit(RenderCommand{mesh.transform, mesh.object, mesh.object->GetMaterial(), 0});
        } */

        auto animatorView = m_Registry.view<AnimatorComponent>();

        for (auto& entity : animatorView)
        {
            AnimatorComponent* animatorComponent = &animatorView.get<AnimatorComponent>(entity);
            AnimationSystem::Update(dt, animatorComponent);
        }

        auto viewRigidbody = m_Registry.view<RigidbodyComponent, TransformComponent>();

        for (auto entity : viewRigidbody) {
            auto [rb, transform] = viewRigidbody.get<RigidbodyComponent, TransformComponent>(entity);
            if (rb.rb) {
                rb.rb->SetPosition(transform.Position);
                rb.rb->SetRotation(transform.Rotation);
            }
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


        // Get all entities with ParticlesSystemComponent and TransformComponent
        auto particleSystemView = m_Registry.view<ParticlesSystemComponent, TransformComponent>();
        for (auto& entity : particleSystemView)
        {
            auto& particlesSystemComponent = particleSystemView.get<ParticlesSystemComponent>(entity);
            auto& transformComponent = particleSystemView.get<TransformComponent>(entity);


            auto materialComponent = m_Registry.try_get<MaterialComponent>(entity);
            Ref<Material> material = (materialComponent) ? materialComponent->material : nullptr;

            if (!particlesSystemComponent.GetParticleEmitter()->particleMaterial && material)
            {
                particlesSystemComponent.GetParticleEmitter()->particleMaterial = material;
            }

            particlesSystemComponent.GetParticleEmitter()->transformComponentMatrix = transformComponent.GetWorldTransform();
            particlesSystemComponent.GetParticleEmitter()->cameraViewMatrix = glm::inverse(cameraTransform);
            particlesSystemComponent.GetParticleEmitter()->Update(dt);

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


        // Clear collision system state
        CollisionSystem::Shutdown();        
        Audio::StopAllEvents();
    }

    Ref<Scene> Scene::Load(const std::filesystem::path& path)
    {
        ZoneScoped;
    
        Ref<Scene> scene = CreateRef<Scene>();
    
        std::ifstream sceneFile(path);
        cereal::JSONInputArchive archive(sceneFile);

        archive(*scene);
        
        scene->m_FilePath = path;
        
        // TODO: Think where this could be done instead of the Load function

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
        
        // TODO: Think where this could be done instead of the Load function
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

        archive(*scene);
        
        scene->m_FilePath = path;
    }

    // Is possible that this function will be moved to the SceneTreePanel but for now it will stay here
    void AddModelToTheSceneTree(Scene* scene, Ref<Model> model, AnimatorComponent* animatorComponent)
    {
        static Entity parent;

        Entity modelEntity = scene->CreateEntity(model->GetName());

        if (model->HasAnimations())
        {
            animatorComponent = &modelEntity.AddComponent<AnimatorComponent>(model->GetSkeleton(), model->GetAnimationController());
            AnimationSystem::SetCurrentAnimation(0, animatorComponent);
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

        parent = Entity{entt::null, scene};
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

    void Scene::OnEditorUpdateUI(float dt, entt::registry& registry) {
        auto windowSize = Renderer::GetCurrentRenderTarget()->GetSize();
        glm::vec2 center = glm::vec2(windowSize.x / 2.0f, windowSize.y / 2.0f);
        //center = glm::vec2(0, 0);

        auto uiImageView = registry.view<UIImageComponent, TransformComponent>();
        for (auto& entity : uiImageView) {
            auto& uiImageComponent = uiImageView.get<UIImageComponent>(entity);
            auto& transformComponent = uiImageView.get<TransformComponent>(entity);

            if (!uiImageComponent.Visible || !uiImageComponent.texture)
                continue;

            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform[3] = glm::vec4(transform[3][0] + center.x, -transform[3][1] + center.y, 0, 0);
            transform = glm::scale(transform, glm::vec3(uiImageComponent.Size.x, -uiImageComponent.Size.y, 1.0f));

            Renderer2D::DrawQuad(transform,
                uiImageComponent.texture,
                1.0f,                // Tiling factor
                glm::vec4(1.0f),     // Tint color
                Renderer2D::RenderMode::Screen,  // Rendering Mode
                (uint32_t)entity     // Entity ID
            );
        }


        auto uiTextView = registry.view<UITextComponent, TransformComponent>();
        for (auto& entity : uiTextView) {
            auto& uiTextComponent = uiTextView.get<UITextComponent>(entity);
            auto& transformComponent = uiTextView.get<TransformComponent>(entity);

            if (!uiTextComponent.Visible || uiTextComponent.Text.empty())
                continue;

            if (!uiTextComponent.font) {
                uiTextComponent.font = Font::GetDefault();
            }
            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform[3] = glm::vec4(transform[3][0] + center.x, -transform[3][1] + center.y, 0, 0);
            transform = glm::scale(transform, glm::vec3(uiTextComponent.FontSize, -uiTextComponent.FontSize, 1.0f));

            Renderer2D::DrawString(
                uiTextComponent.Text,
                uiTextComponent.font,
                transform,
                {uiTextComponent.Color, 0.0f, 0.0f},
                Renderer2D::RenderMode::Screen,
                (uint32_t)entity
            );
        }


        auto uiSliderView = registry.view<UISliderComponent, TransformComponent>();
        for (auto& entity : uiSliderView) {
            auto& uiSliderComponent = uiSliderView.get<UISliderComponent>(entity);
            auto& transformComponent = uiSliderView.get<TransformComponent>(entity);

            if (!uiSliderComponent.Visible) continue;

            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform[3] = glm::vec4(transform[3][0] + center.x, -transform[3][1] + center.y, 0, 0);

            glm::mat4 barTransform = glm::scale(transform, glm::vec3(uiSliderComponent.Size.x, uiSliderComponent.Size.y, 1.0f));

            if (uiSliderComponent.barTexture) {
                Renderer2D::DrawQuad(
                    barTransform,
                    uiSliderComponent.barTexture,
                    1.0f,
                    glm::vec4(1.0f),
                    Renderer2D::RenderMode::Screen,
                    (uint32_t)entity
                );
            }

            if (uiSliderComponent.handleTexture) {
                float normalizedValue = glm::clamp(uiSliderComponent.Value, 0.0f, 1.0f);
                float handleOffset = normalizedValue * (uiSliderComponent.Size.x - uiSliderComponent.HandleSize.x);
                handleOffset -= (uiSliderComponent.Size.x / 2.0f) - (uiSliderComponent.HandleSize.x / 2.0f);

                glm::mat4 handleTransform = glm::translate(transform, glm::vec3(handleOffset, 0.0f, 0.0f));
                handleTransform = glm::scale(handleTransform, glm::vec3(uiSliderComponent.HandleSize.x, uiSliderComponent.HandleSize.y, 1.0f));

                Renderer2D::DrawQuad(
                    handleTransform,
                    uiSliderComponent.handleTexture,
                    1.0f,
                    glm::vec4(1.0f),
                    Renderer2D::RenderMode::Screen,
                    (uint32_t)entity
                );
            }
        }

        auto uiButtonView = registry.view<UIButtonComponent, TransformComponent>();
        for (auto& entity : uiButtonView) {
            auto& uiButtonComponent = uiButtonView.get<UIButtonComponent>(entity);
            auto& transformComponent = uiButtonView.get<TransformComponent>(entity);

            // Añadir comprobación de textura válida
            Ref<Texture2D> currentTexture = uiButtonComponent.GetCurrentTexture();
            if(!currentTexture) continue;

            // Calcular transformación segura
            glm::mat4 transform = glm::mat4(1.0f);
            try {
                transform = transformComponent.GetWorldTransform();
                transform[3] = glm::vec4(transform[3][0] + center.x, -transform[3][1] + center.y, 0, 0);
                transform = glm::scale(transform, glm::vec3(
                    glm::max(uiButtonComponent.GetCurrentSize().x, 0.1f),
                    glm::max(uiButtonComponent.GetCurrentSize().y, 0.1f),
                    1.0f
                ));
            }
            catch(...) {
                COFFEE_CORE_ERROR("Invalid transform for button entity {}", (uint32_t)entity);
                continue;
            }

            Renderer2D::DrawQuad(
                transform,
                currentTexture,
                1.0f,
                uiButtonComponent.GetCurrentColor(),
                Renderer2D::RenderMode::Screen,
                (uint32_t)entity
            );
        }
    }

    void Scene::OnRuntimeUpdateUI(float dt, entt::registry& registry)
    {
        auto windowSize = Renderer::GetCurrentRenderTarget()->GetSize();
        glm::vec2 center = glm::vec2(windowSize.x / 2.0f, windowSize.y / 2.0f);


        auto uiImageView = registry.view<UIImageComponent, TransformComponent>();
        for (auto& entity : uiImageView) {
            auto& uiImageComponent = uiImageView.get<UIImageComponent>(entity);
            auto& transformComponent = uiImageView.get<TransformComponent>(entity);

            if (!uiImageComponent.Visible || !uiImageComponent.texture)
                continue;

            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform[3] = glm::vec4(transform[3][0] + center.x, -transform[3][1] + center.y, 0, 0);
            transform = glm::scale(transform, glm::vec3(uiImageComponent.Size.x, -uiImageComponent.Size.y, 1.0f));

            Renderer2D::DrawQuad(transform,
                uiImageComponent.texture,
                1.0f,                // Tiling factor
                glm::vec4(1.0f),     // Tint color
                Renderer2D::RenderMode::Screen,  // Rendering Mode
                (uint32_t)entity     // Entity ID
            );
        }


        auto uiTextView = registry.view<UITextComponent, TransformComponent>();
        for (auto& entity : uiTextView) {
            auto& uiTextComponent = uiTextView.get<UITextComponent>(entity);
            auto& transformComponent = uiTextView.get<TransformComponent>(entity);

            if (!uiTextComponent.Visible || uiTextComponent.Text.empty())
                continue;

            if (!uiTextComponent.font) {
                uiTextComponent.font = Font::GetDefault();
            }
            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform[3] = glm::vec4(transform[3][0] + center.x, -transform[3][1] + center.y, 0, 0);
            transform = glm::scale(transform, glm::vec3(uiTextComponent.FontSize, -uiTextComponent.FontSize, 1.0f));

            Renderer2D::DrawString(
                uiTextComponent.Text,
                uiTextComponent.font,
                transform,
                {uiTextComponent.Color, 0.0f, 0.0f},
                Renderer2D::RenderMode::Screen,
                (uint32_t)entity
            );
        }


        auto uiSliderView = registry.view<UISliderComponent, TransformComponent>();
        for (auto& entity : uiSliderView) {
            auto& uiSliderComponent = uiSliderView.get<UISliderComponent>(entity);
            auto& transformComponent = uiSliderView.get<TransformComponent>(entity);

            if (!uiSliderComponent.Visible) continue;

            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform[3] = glm::vec4(transform[3][0] + center.x, -transform[3][1] + center.y, 0, 0);

            glm::mat4 barTransform = glm::scale(transform, glm::vec3(uiSliderComponent.Size.x, uiSliderComponent.Size.y, 1.0f));

            if (uiSliderComponent.barTexture) {
                Renderer2D::DrawQuad(
                    barTransform,
                    uiSliderComponent.barTexture,
                    1.0f,
                    glm::vec4(1.0f),
                    Renderer2D::RenderMode::Screen,
                    (uint32_t)entity
                );
            }

            if (uiSliderComponent.handleTexture) {
                float normalizedValue = glm::clamp(uiSliderComponent.Value, 0.0f, 1.0f);
                float handleOffset = normalizedValue * (uiSliderComponent.Size.x - uiSliderComponent.HandleSize.x);
                handleOffset -= (uiSliderComponent.Size.x / 2.0f) - (uiSliderComponent.HandleSize.x / 2.0f);

                glm::mat4 handleTransform = glm::translate(transform, glm::vec3(handleOffset, 0.0f, 0.0f));
                handleTransform = glm::scale(handleTransform, glm::vec3(uiSliderComponent.HandleSize.x, uiSliderComponent.HandleSize.y, 1.0f));

                Renderer2D::DrawQuad(
                    handleTransform,
                    uiSliderComponent.handleTexture,
                    1.0f,
                    glm::vec4(1.0f),
                    Renderer2D::RenderMode::Screen,
                    (uint32_t)entity
                );
            }
        }

        auto uiButtonView = registry.view<UIButtonComponent, TransformComponent>();
        for (auto& entity : uiButtonView) {
            auto& uiButtonComponent = uiButtonView.get<UIButtonComponent>(entity);
            auto& transformComponent = uiButtonView.get<TransformComponent>(entity);

            // Añadir comprobación de textura válida
            Ref<Texture2D> currentTexture = uiButtonComponent.GetCurrentTexture();
            if(!currentTexture) continue;

            // Calcular transformación segura
            glm::mat4 transform = glm::mat4(1.0f);
            try {
                transform = transformComponent.GetWorldTransform();
                transform[3] = glm::vec4(transform[3][0] + center.x, -transform[3][1] + center.y, 0, 0);
                transform = glm::scale(transform, glm::vec3(
                    glm::max(uiButtonComponent.GetCurrentSize().x, 0.1f),
                    glm::max(uiButtonComponent.GetCurrentSize().y, 0.1f),
                    1.0f
                ));
            }
            catch(...) {
                COFFEE_CORE_ERROR("Invalid transform for button entity {}", (uint32_t)entity);
                continue;
            }

            Renderer2D::DrawQuad(
                transform,
                currentTexture,
                1.0f,
                uiButtonComponent.GetCurrentColor(),
                Renderer2D::RenderMode::Screen,
                (uint32_t)entity
            );
        }
    }
}
