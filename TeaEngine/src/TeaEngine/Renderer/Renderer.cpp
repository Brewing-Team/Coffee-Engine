#include "Renderer.h"
#include "TeaEngine/Renderer/DebugRenderer.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Renderer/Framebuffer.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include "TeaEngine/Renderer/UniformBuffer.h"
#include <cstdint>
#include <glm/fwd.hpp>
#include <glm/matrix.hpp>
#include <tracy/Tracy.hpp>

namespace Tea {

    RendererData Renderer::s_RendererData;
    RendererStats Renderer::s_Stats;
    RenderSettings Renderer::s_RenderSettings;
    Ref<Framebuffer> Renderer::s_MainFramebuffer;

    void Renderer::Init()
    {
        ZoneScoped;

        RendererAPI::Init();
        DebugRenderer::Init();

        s_RendererData.CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);

        s_MainFramebuffer = Framebuffer::Create(1280, 720, {ImageFormat::RGBA8, ImageFormat::DEPTH24STENCIL8});
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

        s_MainFramebuffer->Bind();

        RendererAPI::SetClearColor({.1f,.1f,.1f,1});
        RendererAPI::Clear();
    }

    void Renderer::EndScene()
    {
        if(s_RenderSettings.PostProcessing)
        {
            //Render All the fancy effects :D
        }

        //Final Pass
        s_RendererData.RenderTexture = s_MainFramebuffer->GetColorTexture();

        s_MainFramebuffer->UnBind();
    }

    void Renderer::BeginOverlay(EditorCamera& camera)
    {
        s_RendererData.cameraData.view = camera.GetViewMatrix();
        s_RendererData.cameraData.projection = camera.GetProjection();
        s_RendererData.cameraData.position = camera.GetPosition();
        s_RendererData.CameraUniformBuffer->SetData(&s_RendererData.cameraData, sizeof(RendererData::CameraData));

        s_MainFramebuffer->Bind();
    }

    void Renderer::EndOverlay()
    {
        s_MainFramebuffer->UnBind();
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

    void Renderer::OnResize(uint32_t width, uint32_t height)
    {
        s_MainFramebuffer->Resize(width, height);
    }
}