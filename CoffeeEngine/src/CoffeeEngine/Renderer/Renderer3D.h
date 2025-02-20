#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/EditorCamera.h"
#include "CoffeeEngine/Renderer/Framebuffer.h"
#include "CoffeeEngine/Renderer/Material.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/Renderer/UniformBuffer.h"
#include "CoffeeEngine/Renderer/VertexArray.h"
#include "CoffeeEngine/Scene/Components.h"
#include <glm/fwd.hpp>

namespace Coffee {

    /**
     * @defgroup renderer Renderer
     * @brief Renderer components of the CoffeeEngine.
     * @{
     */

    struct RenderCommand
    {
        glm::mat4 transform;
        Ref<Mesh> mesh;
        Ref<Material> material;
        uint32_t entityID;
    };

    /**
     * @brief Structure containing renderer data.
     */
    struct Renderer3DData
    {
        /**
         * @brief Structure containing render data.
         */
        struct SceneRenderData
        {
            LightComponent lights[32]; ///< Array of light components.
            int lightCount = 0; ///< Number of lights.
        };

        SceneRenderData renderData; ///< Render data.

        Ref<UniformBuffer> SceneRenderDataUniformBuffer; ///< Uniform buffer for render data.

        Ref<Material> DefaultMaterial; ///< Default material.
        Ref<Mesh> MissingMesh; ///< Missing mesh.

        std::vector<RenderCommand> renderQueue; ///< Render queue.
    };

    /**
     * @brief Structure containing renderer statistics.
     */
    struct RendererStats
    {
        uint32_t DrawCalls = 0; ///< Number of draw calls.
        uint32_t VertexCount = 0; ///< Number of vertices.
        uint32_t IndexCount = 0; ///< Number of indices.
    };

    /**
     * @brief Structure containing render settings.
     */
    struct RenderSettings
    {
        bool PostProcessing = true; ///< Enable or disable post-processing.
        bool SSAO = false; ///< Enable or disable SSAO.
        bool Bloom = false; ///< Enable or disable bloom.
        bool FXAA = false; ///< Enable or disable FXAA.
        float Exposure = 1.0f; ///< Exposure value.

        // REMOVE: This is for the first release of the engine it should be handled differently
        bool showNormals = false;
    };

    /**
     * @brief Class representing the 3D renderer.
     */
    class Renderer3D
    {
    public:
        /**
         * @brief Initializes the renderer.
         */
        static void Init();

        /**
         * @brief Shuts down the renderer.
         */
        static void Shutdown();

        /**
         * @brief Begins a new scene with the specified editor camera.
         * @param camera The editor camera.
         */
        static void BeginScene(EditorCamera& camera);

        /**
         * @brief Begins a new scene with the specified camera and transform.
         * @param camera The camera.
         * @param transform The transform matrix.
         */
        static void BeginScene(Camera& camera, const glm::mat4& transform);

        /**
         * @brief Ends the current scene.
         */
        static void EndScene();

        /**
         * @brief Begins an overlay with the specified editor camera.
         * @param camera The editor camera.
         */
        static void BeginOverlay(EditorCamera& camera);

        /**
         * @brief Ends the current overlay.
         */
        static void EndOverlay();

        static void Submit(const RenderCommand& command);

        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f), uint32_t entityID = 4294967295);

        /**
         * @brief Submits a light component.
         * @param light The light component.
         */

         //Todo change this to a light class and not a component
        static void Submit(const LightComponent& light);

        /**
         * @brief Resizes the renderer to the specified width and height.
         * @param width The new width.
         * @param height The new height.
         */
        static void OnResize(uint32_t width, uint32_t height);

        /**
         * @brief Gets the renderer data.
         * @return A reference to the renderer data.
         */
        static const Renderer3DData& GetData() { return s_RendererData; }

        /**
         * @brief Gets the renderer statistics.
         * @return A reference to the renderer statistics.
         */
        static const RendererStats& GetStats() { return s_Stats; }

        /**
         * @brief Gets the render settings.
         * @return A reference to the render settings.
         */
        static RenderSettings& GetRenderSettings() { return s_RenderSettings; }

    private:

        static void ResizeFramebuffers();

    private:
        static Renderer3DData s_RendererData; ///< Renderer data.
        static RendererStats s_Stats; ///< Renderer statistics.
        static RenderSettings s_RenderSettings; ///< Render settings.

        static Ref<Texture2D> s_MainRenderTexture; ///< Main render texture.
        static Ref<Texture2D> s_EntityIDTexture; ///< Entity ID texture.
        static Ref<Texture2D> s_PostProcessingTexture; ///< Post-processing texture.
        static Ref<Texture2D> s_DepthTexture; ///< Depth texture.

        static Ref<Framebuffer> s_MainFramebuffer; ///< Main framebuffer.
        static Ref<Framebuffer> s_PostProcessingFramebuffer; ///< Post-processing framebuffer.

        static Ref<Shader> s_ToneMappingShader; ///< Tone mapping shader.
        static Ref<Shader> s_FinalPassShader; ///< Final pass shader.
    };

    /** @} */
}