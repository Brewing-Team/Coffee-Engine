#pragma once

#include "CoffeeEngine/Renderer/Mesh.h"
#include "CoffeeEngine/Renderer/RenderTarget.h"
#include "CoffeeEngine/Renderer/UniformBuffer.h"
#include <glm/fwd.hpp>
#include <vector>
namespace Coffee {

    struct CameraData
    {
        glm::mat4 projection = glm::mat4(1.0f); ///< The projection matrix.
        glm::mat4 view = glm::mat4(1.0f); ///< The view matrix.
        glm::vec3 position = {0.0, 0.0, 0.0}; ///< The position of the camera.
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

        // TEMPORAL - Animation
        static void Update(float dt) { if (m_AnimationSystem) m_AnimationSystem->Update(dt); }

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

    public:
        // TEMPORAL - Animation
        static Ref<AnimationSystem> m_AnimationSystem;

    private:
        static RendererData s_RendererData; ///< Renderer data.
        static RendererSettings s_RenderSettings; ///< Render settings.
        static Ref<Mesh> s_ScreenQuad; ///< Screen quad mesh.
    };

}