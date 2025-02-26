#include "Renderer.h"
#include "Renderer3D.h"
#include "Renderer2D.h"
#include "CoffeeEngine/Renderer/RendererAPI.h"
#include "CoffeeEngine/Scene/PrimitiveMesh.h"

#include <glm/matrix.hpp>
#include <tracy/Tracy.hpp>
#include <vector>

namespace Coffee {

    static bool s_viewportResized = false;
    static uint32_t s_viewportWidth = 0, s_viewportHeight = 0;

    RendererData Renderer::s_RendererData;
    RendererSettings Renderer::s_RenderSettings;

    Ref<Mesh> Renderer::s_ScreenQuad;

    void Renderer::Init()
    {
        ZoneScoped;

        RendererAPI::Init();

        Renderer2D::Init();
        Renderer3D::Init();

        Ref<Shader> missingShader = CreateRef<Shader>("MissingShader", std::string(missingShaderSource));
        s_RendererData.DefaultMaterial = CreateRef<Material>("Missing Material", missingShader); //TODO: Port it to use the Material::Create

        // TODO: This is a hack to get the missing mesh add it to the PrimitiveMesh class
        Ref<Model> m = Model::Load("assets/models/MissingMesh.glb");
        s_RendererData.MissingMesh = m->GetMeshes()[0];

        s_MainFramebuffer = Framebuffer::Create(1280, 720, { ImageFormat::RGBA32F, ImageFormat::RGB8, ImageFormat::DEPTH24STENCIL8 });
        s_PostProcessingFramebuffer = Framebuffer::Create(1280, 720, { ImageFormat::RGBA8 });

        s_MainRenderTexture = s_MainFramebuffer->GetColorTexture(0);
        s_EntityIDTexture = s_MainFramebuffer->GetColorTexture(1);
        s_DepthTexture = s_MainFramebuffer->GetDepthTexture();

        s_PostProcessingTexture = s_PostProcessingFramebuffer->GetColorTexture(0);

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
            
            material->Use();

            const Ref<Shader>& shader = material->GetShader();

            shader->Bind();

            if (command.animation)
                command.animation->SetBoneTransformations(shader);
            else
                shader->setBool("animated", false);

            shader->setMat4("model", command.transform);
            shader->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(command.transform))));

            //REMOVE: This is for the first release of the engine it should be handled differently
            shader->setBool("showNormals", s_RenderSettings.showNormals);

            // Convert entityID to vec3
            uint32_t r = (command.entityID & 0x000000FF) >> 0;
            uint32_t g = (command.entityID & 0x0000FF00) >> 8;
            uint32_t b = (command.entityID & 0x00FF0000) >> 16;
            glm::vec3 entityIDVec3 = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);

            shader->setVec3("entityID", entityIDVec3);

            Mesh* mesh = command.mesh.get();
            
            if(mesh == nullptr)
            {
                mesh = s_RendererData.MissingMesh.get();
            }
            
            RendererAPI::DrawIndexed(mesh->GetVertexArray());

            s_Stats.DrawCalls++;

            s_Stats.VertexCount += mesh->GetVertices().size();
            s_Stats.IndexCount += mesh->GetIndices().size();
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