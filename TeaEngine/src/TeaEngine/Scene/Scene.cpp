#include "Scene.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Core/Log.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include "TeaEngine/Scene/Components.h"
#include "TeaEngine/Scene/Entity.h"
#include "TeaEngine/Renderer/Shader.h"

namespace Tea {

    Scene::Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        return entity;
    }

    void Scene::OnInit()
    {
        for(int i = 0; i < 10; i++){
            Entity e = CreateEntity("Entity Test");
            
            e.AddComponent<ModelComponent>("assets/models/DamagedHelmet.glb");
        }
    }

    void Scene::OnUpdate()
    {
       
    }

    void Scene::OnUpdateEditor(EditorCamera& camera)
    {
        //-------------- TEMPORAL ------------------
        Ref<Shader> s = Tea::Shader::Create("assets/shaders/FaceIndexShader.vert", "assets/shaders/FaceIndexShader.frag");

        s->Bind();

        s->setMat4("view", camera.GetViewMatrix());
        s->setMat4("projection", camera.GetProjection());

        //------------------------------------------

        // Get all entities with ModelComponent and TransformComponent
        auto view = m_Registry.view<ModelComponent, TransformComponent>();

        // Loop through each entity with the specified components
        for (auto entity : view)
        {
            // Get the ModelComponent and TransformComponent for the current entity
            auto& modelComponent = view.get<ModelComponent>(entity);
            auto& transformComponent = view.get<TransformComponent>(entity);
            
            s->setMat4("model", transformComponent.GetTransform()); //TEMPORAL

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