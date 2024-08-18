#include "Renderer.h"
#include "TeaEngine/Renderer/DebugRenderer.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include "TeaEngine/Renderer/UniformBuffer.h"
#include <glm/fwd.hpp>
#include <glm/matrix.hpp>
#include <tracy/Tracy.hpp>

namespace Tea {

    RendererData Renderer::s_RendererData;
    RendererStats Renderer::s_Stats;
    RenderSettings Renderer::s_RenderSettings;

    void Renderer::Init()
    {
        ZoneScoped;

        RendererAPI::Init();
        DebugRenderer::Init();

        s_RendererData.CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);
    }

    void Renderer::Shutdown()
    {
    }

    void Renderer::BeginScene(EditorCamera& camera)
    {
        s_Stats.DrawCalls = 0;
        s_Stats.VertexCount = 0;
        s_Stats.IndexCount = 0;

        s_RendererData.cameraData.view = camera.GetViewMatrix();
        s_RendererData.cameraData.projection = camera.GetProjection();
        s_RendererData.cameraData.position = camera.GetPosition();
        s_RendererData.CameraUniformBuffer->SetData(&s_RendererData.cameraData, sizeof(RendererData::CameraData));
    }

    void Renderer::EndScene()
    {
        if(s_RenderSettings.PostProcessing)
        {
            //Render All the fancy effects :D
        }
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();
        shader->setMat4("model", transform);
        shader->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(transform))));

        RendererAPI::DrawIndexed(vertexArray);

        s_Stats.DrawCalls++;
    }

    void Renderer::Submit(const Ref<Material>& material, const Ref<Mesh>& mesh, const glm::mat4& transform)
    {
        material->Use();
        Ref<Shader> shader = material->GetShader();

        Renderer::Submit(shader, mesh->GetVertexArray(), transform);

        s_Stats.VertexCount += mesh->GetVertices().size();
        s_Stats.IndexCount += mesh->GetIndices().size();
    }
}