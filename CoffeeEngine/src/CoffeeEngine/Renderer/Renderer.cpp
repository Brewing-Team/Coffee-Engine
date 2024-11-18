#include "Renderer.h"
#include "CoffeeEngine/Scene/PrimitiveMesh.h"
#include "CoffeeEngine/Renderer/DebugRenderer.h"
#include "CoffeeEngine/Renderer/EditorCamera.h"
#include "CoffeeEngine/Renderer/Framebuffer.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include "CoffeeEngine/Renderer/RendererAPI.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/Renderer/UniformBuffer.h"

#include "CoffeeEngine/Embedded/ToneMappingShader.inl"
#include "CoffeeEngine/Embedded/FinalPassShader.inl"

#include <cstdint>
#include <glm/fwd.hpp>
#include <glm/matrix.hpp>
#include <tracy/Tracy.hpp>

namespace Coffee {

    static bool s_viewportResized = false;
    static uint32_t s_viewportWidth = 0, s_viewportHeight = 0;

    RendererData Renderer::s_RendererData;
    RendererStats Renderer::s_Stats;
    RenderSettings Renderer::s_RenderSettings;
    Ref<Framebuffer> Renderer::s_MainFramebuffer;
    Ref<Framebuffer> Renderer::s_PostProcessingFramebuffer;
    Ref<Texture> Renderer::s_MainRenderTexture;
    Ref<Texture> Renderer::s_EntityIDTexture;
    Ref<Texture> Renderer::s_PostProcessingTexture;
    Ref<Texture> Renderer::s_DepthTexture;

    Ref<Mesh> Renderer::s_ScreenQuad;

    Ref<Shader> Renderer::s_ToneMappingShader;
    Ref<Shader> Renderer::s_FinalPassShader;

    void Renderer::Init()
    {
        ZoneScoped;

        RendererAPI::Init();
        DebugRenderer::Init();

        s_RendererData.CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);
        s_RendererData.RenderDataUniformBuffer = UniformBuffer::Create(sizeof(RendererData::RenderData), 1);

        s_MainFramebuffer = Framebuffer::Create(1280, 720, { ImageFormat::RGBA32F, ImageFormat::RGB8, ImageFormat::DEPTH24STENCIL8 });
        s_PostProcessingFramebuffer = Framebuffer::Create(1280, 720, { ImageFormat::RGBA8 });

        s_MainRenderTexture = s_MainFramebuffer->GetColorTexture(0);
        s_EntityIDTexture = s_MainFramebuffer->GetColorTexture(1);
        s_DepthTexture = s_MainFramebuffer->GetDepthTexture();

        s_PostProcessingTexture = s_PostProcessingFramebuffer->GetColorTexture(0);

        s_ScreenQuad = PrimitiveMesh::CreateQuad();

        s_ToneMappingShader = CreateRef<Shader>(std::string(toneMappingShaderSource));
        s_FinalPassShader = CreateRef<Shader>(std::string(finalPassShaderSource));
    }

    void Renderer::Shutdown()
    {
    }

    void Renderer::BeginScene(EditorCamera& camera)
    {
        s_Stats.DrawCalls = 0;
        s_Stats.VertexCount = 0;
        s_Stats.IndexCount = 0;

        //I think if a render queue is implemented this is not necessary. The OnResize would work.
        if(s_viewportResized)
        {
            ResizeFramebuffers();
            s_viewportResized = false;
        }

        s_RendererData.cameraData.view = camera.GetViewMatrix();
        s_RendererData.cameraData.projection = camera.GetProjection();
        s_RendererData.cameraData.position = camera.GetPosition();
        s_RendererData.CameraUniformBuffer->SetData(&s_RendererData.cameraData, sizeof(RendererData::CameraData));

        //This is setting the light information from the previous frame :(
        s_RendererData.RenderDataUniformBuffer->SetData(&s_RendererData.renderData, sizeof(RendererData::RenderData));
        s_RendererData.renderData.lightCount = 0;

        s_MainFramebuffer->Bind();
        s_MainFramebuffer->SetDrawBuffers({0, 1});

        RendererAPI::SetClearColor({0.03f,0.03f,0.03f,1.0});
        RendererAPI::Clear();

        s_EntityIDTexture->Clear({-1.0f,0.0f,0.0f,0.0f});
    }

    void Renderer::BeginScene(Camera& camera, const glm::mat4& transform)
    {
        s_Stats.DrawCalls = 0;
        s_Stats.VertexCount = 0;
        s_Stats.IndexCount = 0;

        // This resize the camera to the viewport size. Think how to manage this in a better way :p
        camera.SetViewportSize(s_viewportWidth, s_viewportHeight);

        s_RendererData.cameraData.view = glm::inverse(transform);
        s_RendererData.cameraData.projection = camera.GetProjection();
        s_RendererData.cameraData.position = transform[3];
        s_RendererData.CameraUniformBuffer->SetData(&s_RendererData.cameraData, sizeof(RendererData::CameraData));

        //This is setting the light information from the previous frame :(
        s_RendererData.RenderDataUniformBuffer->SetData(&s_RendererData.renderData, sizeof(RendererData::RenderData));
        s_RendererData.renderData.lightCount = 0;

        s_MainFramebuffer->Bind();

        RendererAPI::SetClearColor({0.03f,0.03f,0.03f,1.0});
        RendererAPI::Clear();
    }

    void Renderer::EndScene()
    {
        if(s_RenderSettings.PostProcessing)
        {
            //Render All the fancy effects :D

            //ToneMapping
            s_PostProcessingFramebuffer->Bind();

            s_ToneMappingShader->Bind();
            s_ToneMappingShader->setInt("screenTexture", 0);
            s_ToneMappingShader->setFloat("exposure", s_RenderSettings.Exposure);
            s_MainRenderTexture->Bind(0);

            RendererAPI::DrawIndexed(s_ScreenQuad->GetVertexArray());

            s_ToneMappingShader->Unbind();

            //This has to be set because the s_ScreenQuad overwrites the depth buffer
            RendererAPI::SetDepthMask(false);

            //Final Pass
            s_MainFramebuffer->Bind();
            s_MainFramebuffer->SetDrawBuffers({0});
            
            s_FinalPassShader->Bind();
            s_FinalPassShader->setInt("screenTexture", 0);
            s_PostProcessingTexture->Bind(0);

            RendererAPI::DrawIndexed(s_ScreenQuad->GetVertexArray());

            s_FinalPassShader->Unbind();

            RendererAPI::SetDepthMask(true);

            //std::swap(s_PostProcessingTexture, s_MainRenderTexture);
        }

        //Final Pass
        s_RendererData.RenderTexture = s_MainRenderTexture;

        s_MainFramebuffer->UnBind();
    }

    //TEMPORAL
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

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, uint32_t entityID)
    {
        shader->Bind();
        shader->setMat4("model", transform);
        shader->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(transform))));

        //REMOVE: This is for the first release of the engine it should be handled differently
        shader->setBool("showNormals", s_RenderSettings.showNormals);

        // Convert entityID to vec3
        uint32_t r = (entityID & 0x000000FF) >> 0;
        uint32_t g = (entityID & 0x0000FF00) >> 8;
        uint32_t b = (entityID & 0x00FF0000) >> 16;
        glm::vec3 entityIDVec3 = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);

        shader->setVec3("entityID", entityIDVec3);

        RendererAPI::DrawIndexed(vertexArray);

        s_Stats.DrawCalls++;
    }

    void Renderer::Submit(const Ref<Material>& material, const Ref<Mesh>& mesh, const glm::mat4& transform, uint32_t entityID)
    {
        material->Use();
        Ref<Shader> shader = material->GetShader();

        Renderer::Submit(shader, mesh->GetVertexArray(), transform, entityID);

        s_Stats.VertexCount += mesh->GetVertices().size();
        s_Stats.IndexCount += mesh->GetIndices().size();
    }

    void Renderer::Submit(const LightComponent& light)
    {
        s_RendererData.renderData.lights[s_RendererData.renderData.lightCount] = light;
        s_RendererData.renderData.lightCount++;
    }

    void Renderer::OnResize(uint32_t width, uint32_t height)
    {
        s_viewportWidth = width;
        s_viewportHeight = height;

        s_viewportResized = true;
    }

    void Renderer::ResizeFramebuffers()
    {
        s_MainFramebuffer->Resize(s_viewportWidth, s_viewportHeight);
        s_PostProcessingFramebuffer->Resize(s_viewportWidth, s_viewportHeight);
    }
}