#pragma once

#include "CoffeeEngine/Renderer/Mesh.h"
#include "CoffeeEngine/Renderer/RenderTarget.h"
#include "CoffeeEngine/Renderer/UniformBuffer.h"
#include <vector>
namespace Coffee {

    struct RendererData
    {
        std::vector<RenderTarget> RenderTargets;
        
        Ref<UniformBuffer> CameraUniformBuffer; ///< Uniform buffer for camera data.
    };

    class Renderer
    {
    public:
        
        static void Init();
        static void Render();
        static void Shutdown();

        static const RenderTarget& GetRenderTarget(const std::string& name);
        static void AddRenderTarget(const RenderTarget& renderTarget);

        static std::vector<RenderTarget>& GetRenderTargets() { return s_RendererData.RenderTargets; }
    private:
        static RendererData s_RendererData;

        static Ref<Mesh> s_ScreenQuad; ///< Screen quad mesh.
    };

}