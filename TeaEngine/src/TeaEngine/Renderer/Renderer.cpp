#include "Renderer.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include <tracy/Tracy.hpp>

namespace Tea {

    glm::mat4 Tea::Renderer::s_SceneViewMatrix;
    glm::mat4 Tea::Renderer::s_SceneProjectionMatrix;

    void Renderer::Init()
    {
        ZoneScoped;

        RendererAPI::Init();
    }

    void Renderer::Shutdown()
    {
    }

    void Renderer::BeginScene(EditorCamera& camera)
    {
        s_SceneViewMatrix = camera.GetViewMatrix();
        s_SceneProjectionMatrix = camera.GetProjection();
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();
        shader->setMat4("view", s_SceneViewMatrix);
        shader->setMat4("projection", s_SceneProjectionMatrix);
        shader->setMat4("model", transform);

        RendererAPI::DrawIndexed(vertexArray);
    }

    void Renderer::Submit(const Ref<Material>& material, const Ref<Mesh>& mesh, const glm::mat4& transform)
    {
        material->Use();

        Ref<Shader> shader = material->GetShader();
        shader->Bind();
        shader->setMat4("view", s_SceneViewMatrix);
        shader->setMat4("projection", s_SceneProjectionMatrix);
        shader->setMat4("model", transform);
        
        RendererAPI::DrawIndexed(mesh->GetVertexArray());
    }
}