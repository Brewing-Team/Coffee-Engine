#include "Renderer.h"
#include "Renderer3D.h"
#include "CoffeeEngine/Scene/PrimitiveMesh.h"

#include <vector>

namespace Coffee {

    struct CameraData
    {
        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
    };

    RendererData Renderer::s_RendererData;

    void Renderer::Init()
    {
        Renderer3D::Init();

        s_RendererData.CameraUniformBuffer = UniformBuffer::Create(sizeof(CameraData), 0);

        s_ScreenQuad = PrimitiveMesh::CreateQuad();
    }

    void Renderer::Render()
    {
        for (auto& target : s_RendererData.RenderTargets)
        {
            
        }
    }

    void Renderer::Shutdown()
    {
        Renderer3D::Shutdown();
    }

    const RenderTarget& Renderer::GetRenderTarget(const std::string& name)
    {
        for (auto& target : s_RendererData.RenderTargets)
        {
            if (target.GetName() == name)
            {
                return target;
            }
        }

        return s_RendererData.RenderTargets[0];
    }

    void Renderer::AddRenderTarget(const RenderTarget& renderTarget)
    {
        s_RendererData.RenderTargets.push_back(renderTarget);
    }
}