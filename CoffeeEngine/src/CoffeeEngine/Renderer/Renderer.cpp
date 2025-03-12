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

    void Renderer::EndOverlay()
    {
        s_MainFramebuffer->UnBind();
    }

    void Renderer::Submit(const LightComponent& light)
    {
        s_RendererData.renderData.lights[s_RendererData.renderData.lightCount] = light;
        s_RendererData.renderData.lightCount++;
    }

     void Renderer::Submit(Ref<ParticleEmitter> emitter, Ref<Material> material, uint32_t entity)
    {

          for (size_t i = 0; i < emitter->activeParticles.size(); i++)
          {
              Ref<Particle> p = emitter->activeParticles.at(i);
              
                Renderer::Submit(RenderCommand{p->GetWorldTransform(), ParticleEmitter::particleMesh,
                                                material, entity});

          }
    }



    void Renderer::Submit(const RenderCommand& command)
    {
        s_RendererData.renderQueue.push_back(command);
    }

    // Temporal, this should be removed because this is rendering immediately.
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