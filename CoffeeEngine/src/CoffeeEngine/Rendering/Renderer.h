#pragma once

#include "CoffeeEngine/Core/Assert.h"
#include "CoffeeEngine/Rendering/Renderer3D.h"

#include <glm/ext/matrix_float4x4.hpp>

#include <unordered_map>


namespace Coffee {

    class RendererAPI;
    class RenderTarget;
    class UniformBuffer;
    class Mesh;

    struct CameraData
    {
        glm::mat4 projection = glm::mat4(1.0f); ///< The projection matrix.
        glm::mat4 view = glm::mat4(1.0f); ///< The view matrix.
        glm::vec3 position = {0.0, 0.0, 0.0}; ///< The position of the camera.
    };

    struct RendererData
    {
        std::unordered_map<std::string, Ref<RenderTarget>> RenderTargets;
        RenderTarget* CurrentRenderTarget = nullptr;

        Ref<UniformBuffer> CameraUniformBuffer; ///< Uniform buffer for camera data.
        CameraData cameraData; ///< Camera data.
    };

    struct RendererStats
    {
    };

    struct RendererSettings
    {
        bool PostProcessing = true; ///< Enable or disable post-processing.
        float RenderScale = 1.0f; ///< Resolution scale multiplier (0.5 = 50%, 1.0 = 100%, 2.0 = 200%).
    };

    class Renderer
    {
    public:
        
        void Init(RendererAPI* api);
        void Render();
        void Shutdown();

        void AddRenderTarget(const Ref<RenderTarget>& renderTarget);
        void RemoveRenderTarget(const std::string& name);
        Ref<RenderTarget> GetRenderTarget(const std::string& name);

        void SetCurrentRenderTarget(const std::string& name)
        { 
            m_RendererData.CurrentRenderTarget = GetRenderTarget(name).get();
            COFFEE_ASSERT(m_RendererData.CurrentRenderTarget && "Render target not found");
        }

        // This is more dangerous bc the renderTarget can not be in the map but is convenient
        void SetCurrentRenderTarget(RenderTarget* renderTarget)
        {
            m_RendererData.CurrentRenderTarget = renderTarget;
            //COFFEE_ASSERT(m_RendererData.CurrentRenderTarget && "Render target not found");
        }

        RenderTarget* GetCurrentRenderTarget() { return m_RendererData.CurrentRenderTarget; }

        RendererSettings& GetRenderSettings() { return m_RenderSettings; }

    private:
        RendererAPI* m_API; ///< The renderer API used by the renderer.
        Renderer3D m_Renderer3D; ///< The 3D renderer.
        Renderer2D m_Renderer2D; ///< The 2D renderer.

        RendererData m_RendererData; ///< Renderer data.
        RendererSettings m_RenderSettings; ///< Render settings.
        Ref<Mesh> m_ScreenQuad; ///< Screen quad mesh.
    };

}