#include "Renderer.h"
#include "Renderer3D.h"
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
        ZoneScoped;

        RendererAPI::Init();

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
            if (target.GetName() == name)
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