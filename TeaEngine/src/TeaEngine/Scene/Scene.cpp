#include "Scene.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Core/Log.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Renderer/Material.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include "TeaEngine/Renderer/Texture.h"
#include "TeaEngine/Scene/Components.h"
#include "TeaEngine/Scene/Entity.h"
#include "TeaEngine/Renderer/Shader.h"
#include <string>

namespace Tea {

    Scene::Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& nameTag = entity.AddComponent<TagComponent>();
        nameTag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::OnInit()
    {
        Entity e = CreateEntity("Entity Test");
            
        e.AddComponent<ModelComponent>("assets/models/DamagedHelmet.glb");

        for(int i = 0; i < 10; i++)
        {
            Entity e = CreateEntity("Entity(" + std::to_string(i) + ")");
        }
    }

    void Scene::OnUpdate()
    {
       
    }

    void Scene::OnUpdateEditor(EditorCamera& camera)
    {
        //-------------- TEMPORAL ------------------
        MaterialTextures mTextures;
        mTextures.albedo = Texture::Load("assets/textures/test2.jpg");

        Ref<Material> standardMaterial = CreateRef<Material>(mTextures);

        //temporal, in the future use UBO's for this two values
        standardMaterial->SetViewMatrix(camera.GetViewMatrix());
        standardMaterial->SetProjectionMatrix(camera.GetProjection());

        //------------------------------------------

        // Get all entities with ModelComponent and TransformComponent
        auto view = m_Registry.view<ModelComponent, TransformComponent>();

        // Loop through each entity with the specified components
        for (auto entity : view)
        {
            // Get the ModelComponent and TransformComponent for the current entity
            auto& modelComponent = view.get<ModelComponent>(entity);
            auto& transformComponent = view.get<TransformComponent>(entity);
            
            standardMaterial->SetModelMatrix(transformComponent.GetTransform());

            standardMaterial->Use();

            for (auto& mesh : modelComponent.model->GetMeshes())
            {
                RendererAPI::DrawIndexed(mesh->GetVertexArray());
            }
        }
    }

    void Scene::OnEvent(Event& e)
    {

    }

    void Scene::OnExit()
    {

    }

}