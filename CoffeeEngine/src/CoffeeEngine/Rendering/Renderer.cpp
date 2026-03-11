#include "Renderer.h"
#include "CoffeeEngine/Rendering/RenderTarget.h"
#include "Renderer3D.h"
#include "Renderer2D.h"
#include "CoffeeEngine/Rendering/RendererAPI.h"
#include "CoffeeEngine/Scene/PrimitiveMesh.h"
#include "CoffeeEngine/Rendering/UniformBuffer.h"

#include <glm/matrix.hpp>
#include <tracy/Tracy.hpp>
#include <vector>

namespace Coffee {

    void Renderer::Init(RendererAPI* api)
    {
        ZoneScoped;
        
        m_API = api;
        m_API->Init();

        m_Renderer2D.Init(api);
        m_Renderer3D.Init(api);

        m_RendererData.CameraUniformBuffer = UniformBuffer::Create(sizeof(CameraData), 0);

        m_ScreenQuad = PrimitiveMesh::CreateQuad();
    }

    void Renderer::Render()
    {
        ZoneScoped;

        m_Renderer3D.ResetStats();

        for (const auto& [name, target] : m_RendererData.RenderTargets)
        {
            CameraData cameraData;
            cameraData.view = glm::inverse(target->GetCameraTransform());
            cameraData.projection = target->GetCamera().GetProjection();
            cameraData.position = target->GetCameraTransform()[3];

            m_RendererData.CameraUniformBuffer->SetData(&cameraData, sizeof(CameraData));

            m_Renderer3D.ShadowPass(target);
            m_Renderer3D.ForwardPass(target);
            m_Renderer3D.SkyboxPass(target);
            m_Renderer3D.TransparentPass(target);

            if(m_RenderSettings.PostProcessing)
            {
                m_Renderer3D.PostProcessingPass(target);
            }

            // Think if this should be done before or after post processing
            m_Renderer2D.WorldPass(target);
            
            // TODO: Think if this should be done here or in the Renderer2D
            cameraData.projection = glm::ortho(0.0f, target->GetSize().x, target->GetSize().y, 0.0f, -1.0f, 1.0f);
            cameraData.view = glm::mat4(1.0f);
            m_RendererData.CameraUniformBuffer->SetData(&cameraData, sizeof(CameraData));
            
            m_API->SetFaceCulling(false);
            m_API->SetDepthMask(false);

            m_Renderer2D.ScreenPass(target);

            m_API->SetDepthMask(true);
            m_API->SetFaceCulling(true);
        }

        // TODO: Think if this should be done here or inside each target?
        m_Renderer3D.ResetCalls();
    }

    void Renderer::Shutdown()
    {
        m_Renderer3D.Shutdown();
        m_Renderer2D.Shutdown();
    }

    void Renderer::AddRenderTarget(const Ref<RenderTarget>& renderTarget)
    {
        ZoneScoped;

        m_RendererData.RenderTargets[renderTarget->GetName()] = renderTarget;
    }
}