#pragma once

#include "CoffeeEngine/Renderer/Mesh.h"
#include "CoffeeEngine/Renderer/RenderTarget.h"
#include "CoffeeEngine/Renderer/UniformBuffer.h"
#include <glm/fwd.hpp>
#include <vector>
namespace Coffee {

    struct CameraData
    {
        glm::mat4 transform;
        Ref<Mesh> mesh;
        Ref<Material> material;
        uint32_t entityID;
        Ref<AnimationSystem> animation;
    };

    struct RendererData
    {
        std::vector<RenderTarget> RenderTargets;
        RenderTarget* CurrentRenderTarget = nullptr;

        Ref<UniformBuffer> CameraUniformBuffer; ///< Uniform buffer for camera data.
        Ref<UniformBuffer> RenderDataUniformBuffer; ///< Uniform buffer for render data.

        Ref<Material> DefaultMaterial; ///< Default material.
        Ref<Mesh> MissingMesh; ///< Missing mesh.

        Ref<Texture2D> RenderTexture; ///< Render texture.

        std::vector<RenderCommand> renderQueue; ///< Render queue.
    };

    struct RendererStats
    {
    };

    struct RendererSettings
    {
        bool PostProcessing = true; ///< Enable or disable post-processing.
    };

    class Renderer
    {
    public:
        /**
         * @brief Initializes the renderer.
         */
        static void Init();
        static void Render();
        static void Shutdown();

        static const RenderTarget& GetRenderTarget(const std::string& name);
        static RenderTarget& AddRenderTarget(const std::string& name, const glm::vec2& size, const std::vector<std::pair<std::string, std::initializer_list<Attachment>>>& framebufferAttachments);

        static RenderTarget* GetCurrentRenderTarget() { return s_RendererData.CurrentRenderTarget; }
        static void SetCurrentRenderTarget(RenderTarget* renderTarget) { s_RendererData.CurrentRenderTarget = renderTarget; }

        static std::vector<RenderTarget>& GetRenderTargets() { return s_RendererData.RenderTargets; }

        static RendererSettings& GetRenderSettings() { return s_RenderSettings; }

    private:
        static RendererData s_RendererData; ///< Renderer data.
        static RendererSettings s_RenderSettings; ///< Render settings.
        static Ref<Mesh> s_ScreenQuad; ///< Screen quad mesh.
    };

}