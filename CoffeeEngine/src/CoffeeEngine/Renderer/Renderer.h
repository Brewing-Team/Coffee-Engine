#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/EditorCamera.h"
#include "CoffeeEngine/Renderer/Framebuffer.h"
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
    struct RendererData
    {
        /**
         * @brief Structure containing camera data.
         */
        struct CameraData
        {
            glm::mat4 projection; ///< The projection matrix.
            glm::mat4 view; ///< The view matrix.
            glm::vec3 position; ///< The position of the camera.
        };

        /**
         * @brief Structure containing render data.
         */
        struct RenderData
        {
            LightComponent lights[32]; ///< Array of light components.
            int lightCount = 0; ///< Number of lights.
        };

        CameraData cameraData; ///< Camera data.
        RenderData renderData; ///< Render data.

        Ref<UniformBuffer> CameraUniformBuffer; ///< Uniform buffer for camera data.
        Ref<UniformBuffer> RenderDataUniformBuffer; ///< Uniform buffer for render data.

        Ref<Texture> RenderTexture; ///< Render texture.

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
    class Renderer
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
         * @brief Gets the render texture.
         * @return A reference to the render texture.
         */
        static const Ref<Texture>& GetRenderTexture() { return s_RendererData.RenderTexture; }

        /**
         * @brief Retrieves the texture associated with the entity ID.
         * 
         * This static method returns a reference to the texture that is used to 
         * identify entities within the renderer. The texture is stored as a 
         * static member of the class.
         * 
         * @return A constant reference to the entity ID texture.
         */
        static const Ref<Texture>& GetEntityIDTexture() { return s_EntityIDTexture; }

        static glm::vec4 GetEntityIDAtPixel(int x, int y) { return s_MainFramebuffer->GetPixelColor(x, y, 1); }

        /**
         * @brief Gets the renderer data.
         * @return A reference to the renderer data.
         */
        static const RendererData& GetData() { return s_RendererData; }

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
        static RendererData s_RendererData; ///< Renderer data.
        static RendererStats s_Stats; ///< Renderer statistics.
        static RenderSettings s_RenderSettings; ///< Render settings.

        static Ref<Texture> s_MainRenderTexture; ///< Main render texture.
        static Ref<Texture> s_EntityIDTexture; ///< Entity ID texture.
        static Ref<Texture> s_PostProcessingTexture; ///< Post-processing texture.
        static Ref<Texture> s_DepthTexture; ///< Depth texture.

        static Ref<Framebuffer> s_MainFramebuffer; ///< Main framebuffer.
        static Ref<Framebuffer> s_PostProcessingFramebuffer; ///< Post-processing framebuffer.

        static Ref<Mesh> s_ScreenQuad; ///< Screen quad mesh.

        static Ref<Shader> s_ToneMappingShader; ///< Tone mapping shader.
        static Ref<Shader> s_FinalPassShader; ///< Final pass shader.
    };

    /** @} */
}