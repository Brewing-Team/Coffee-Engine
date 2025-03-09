#include "Renderer.h"
#include "Renderer3D.h"
#include "Renderer2D.h"
#include "CoffeeEngine/Renderer/RendererAPI.h"
#include "CoffeeEngine/Scene/PrimitiveMesh.h"

#include <glm/matrix.hpp>
#include <tracy/Tracy.hpp>
#include <vector>

namespace Coffee {
    
    RendererData Renderer::s_RendererData;
    RendererSettings Renderer::s_RenderSettings;

    Ref<Mesh> Renderer::s_ScreenQuad;

    void Renderer::Init()
    {
        /*std::vector<std::filesystem::path> paths = {
            "assets/textures/skybox/right.jpg",
            "assets/textures/skybox/left.jpg",
            "assets/textures/skybox/top.jpg",
            "assets/textures/skybox/bottom.jpg",
            "assets/textures/skybox/front.jpg",
            "assets/textures/skybox/back.jpg"
        };
        s_EnvironmentMap = CreateRef<Cubemap>(paths);*/

        s_EnvironmentMap = /* CreateRef<Cubemap>("assets/textures/StandardCubeMap.hdr"); */ Cubemap::Load("assets/textures/StandardCubeMap.hdr");

        s_SkyboxMesh = PrimitiveMesh::CreateCube({-1.0f, -1.0f, -1.0f});

        s_SkyboxShader = Shader::Create("assets/shaders/SkyboxShader.glsl");

        ZoneScoped;

        RendererAPI::Init();

        Renderer2D::Init();
        Renderer3D::Init();

        s_RendererData.CameraUniformBuffer = UniformBuffer::Create(sizeof(CameraData), 0);

        s_ScreenQuad = PrimitiveMesh::CreateQuad();
    }

    void Renderer::Render()
    {
        ZoneScoped;

        for (auto& target : s_RendererData.RenderTargets)
        {
            CameraData cameraData;
            cameraData.view = glm::inverse(target.GetCameraTransform());
            cameraData.projection = target.GetCamera().GetProjection();
            cameraData.position = target.GetCameraTransform()[3];

            s_RendererData.CameraUniformBuffer->SetData(&cameraData, sizeof(CameraData));

            Renderer3D::ForwardPass(target);
            Renderer3D::SkyboxPass(target);

            if(s_RenderSettings.PostProcessing)
            {
                Renderer3D::PostProcessingPass(target);
            }
            
            // TODO: Think if this should be done here or in the Renderer2D
            cameraData.projection = glm::ortho(0.0f, target.GetSize().x, target.GetSize().y, 0.0f, -1.0f, 1.0f);
            cameraData.view = glm::mat4(1.0f);
            s_RendererData.CameraUniformBuffer->SetData(&cameraData, sizeof(CameraData));
            
            RendererAPI::SetFaceCulling(false);
            RendererAPI::SetDepthMask(false);

            Renderer2D::Render(target);

            RendererAPI::SetDepthMask(true);
            RendererAPI::SetFaceCulling(true);
        }

        // TODO: Think if this should be done here or inside each target?
        Renderer3D::ResetCalls();
    }

    void Renderer::Shutdown()
    {
        Renderer3D::Shutdown();
    }

    const RenderTarget& Renderer::GetRenderTarget(const std::string& name)
    {
        ZoneScoped;

        for (auto& target : s_RendererData.RenderTargets)
        {
            ResizeFramebuffers();
            s_viewportResized = false;
        }

        s_RendererData.cameraData.view = camera.GetViewMatrix();
        s_RendererData.cameraData.projection = camera.GetProjection();
        s_RendererData.cameraData.position = camera.GetPosition();
        s_RendererData.CameraUniformBuffer->SetData(&s_RendererData.cameraData, sizeof(RendererData::CameraData));

        s_RendererData.renderData.lightCount = 0;
    }

    void Renderer::BeginScene(Camera& camera, const glm::mat4& transform)
    {
        s_Stats.DrawCalls = 0;
        s_Stats.VertexCount = 0;
        s_Stats.IndexCount = 0;

        if(s_viewportResized)
        {
            ResizeFramebuffers();
            s_viewportResized = false;
            // This resize the camera to the viewport size. Think how to manage this in a better way :p
            camera.SetViewportSize(s_viewportWidth, s_viewportHeight);
        }

        s_RendererData.cameraData.view = glm::inverse(transform);
        s_RendererData.cameraData.projection = camera.GetProjection();
        s_RendererData.cameraData.position = transform[3];
        s_RendererData.CameraUniformBuffer->SetData(&s_RendererData.cameraData, sizeof(RendererData::CameraData));

        s_RendererData.renderData.lightCount = 0;
    }

    void Renderer::EndScene()
    {
        s_MainFramebuffer->Bind();
        s_MainFramebuffer->SetDrawBuffers({0, 1});

        RendererAPI::SetClearColor({0.03f,0.03f,0.03f,1.0});
        RendererAPI::Clear();

        // Currently this is done also in the runtime, this should be done only in editor mode
        s_EntityIDTexture->Clear({-1.0f,0.0f,0.0f,0.0f});

        s_RendererData.RenderDataUniformBuffer->SetData(&s_RendererData.renderData, sizeof(RendererData::RenderData));

        // Sort the render queue to minimize state changes

        for(const auto& command : s_RendererData.renderQueue)
        {
            Material* material = command.material.get();

            if(material == nullptr)
            {
                return target;
            }
        }

        return s_RendererData.RenderTargets[0];
    }

    RenderTarget& Renderer::AddRenderTarget(const std::string& name, const glm::vec2& size, const std::vector<std::pair<std::string, std::initializer_list<Attachment>>>& framebufferAttachments)
    {
        ZoneScoped;

        s_RendererData.RenderTargets.push_back(RenderTarget(name, size, framebufferAttachments));
        return s_RendererData.RenderTargets.back();
    }
}