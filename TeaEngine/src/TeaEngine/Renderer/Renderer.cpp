#include "Renderer.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include "TeaEngine/Renderer/UniformBuffer.h"
#include <glm/fwd.hpp>
#include <tracy/Tracy.hpp>

namespace Tea {

    struct CameraData
    {
        glm::mat4 projection;
        glm::mat4 view;
        glm::vec3 position;
    }cameraData;

    Ref<UniformBuffer> Renderer::s_CameraUniformBuffer;

    void Renderer::Init()
    {
        ZoneScoped;

        RendererAPI::Init();

        s_CameraUniformBuffer = UniformBuffer::Create(sizeof(CameraData), 0);
    }

    void Renderer::Shutdown()
    {
    }

    void Renderer::BeginScene(EditorCamera& camera)
    {
        cameraData.view = camera.GetViewMatrix();
        cameraData.projection = camera.GetProjection();
        cameraData.position = camera.GetPosition();
        s_CameraUniformBuffer->SetData(&cameraData, sizeof(CameraData));
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();
        shader->setMat4("model", transform);

        RendererAPI::DrawIndexed(vertexArray);
    }

    void Renderer::Submit(const Ref<Material>& material, const Ref<Mesh>& mesh, const glm::mat4& transform)
    {
        material->Use();

        Ref<Shader> shader = material->GetShader();
        shader->Bind();
        shader->setMat4("model", transform);
        
        RendererAPI::DrawIndexed(mesh->GetVertexArray());
    }
}