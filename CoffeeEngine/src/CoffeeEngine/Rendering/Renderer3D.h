#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Scene/Components/LightComponent.h"

#include <glm/matrix.hpp>

namespace Coffee {

    class RenderTarget;
    class UniformBuffer;
    class Mesh;
    class AnimatorComponent;
    class Material;
    class Framebuffer;
    class Cubemap;
    class Shader;
    class Texture2D;
    class VertexArray;
}

namespace Coffee {

    /**
     * @defgroup renderer Renderer
     * @brief Renderer components of the CoffeeEngine.
     * @{
     */

    struct RenderCommand
    {
        glm::mat4 transform = glm::mat4(1.0f);
        Ref<Mesh> mesh;
        Ref<Material> material;
        uint32_t entityID = 4294967295;
    };

    /**
     * @brief Structure containing renderer data.
     */
    struct Renderer3DData
    {
        /**
         * @brief Structure containing render data.
         */

        static constexpr int MAX_DIRECTIONAL_SHADOWS = 4;

        static constexpr int MAX_LIGHTS = 32;

        struct SceneRenderData
        {
            LightComponent lights[MAX_LIGHTS]; ///< Array of light components.
            int lightCount = 0; ///< Number of lights.
            float padding[3]; ///< Padding to align to 16 bytes.
            glm::mat4 LightSpaceMatrices[MAX_DIRECTIONAL_SHADOWS]; ///< Light space matrices for shadow mapping.
        };

        SceneRenderData RenderData; ///< Render data.

        Ref<UniformBuffer> SceneRenderDataUniformBuffer; ///< Uniform buffer for render data.

        Ref<Material> DefaultMaterial; ///< Default material.
        Ref<Mesh> MissingMesh; ///< Missing mesh.
        Ref<Cubemap> DefaultSkybox; ///< Default skybox.

        Ref<Texture2D> BRDFLUT; ///< BRDF LUT texture.

        Ref<Framebuffer> ShadowMapFramebuffer;
        Ref<Texture2D> DirectionalShadowMapTextures[4];

        Ref<Cubemap> EnvironmentMap;

        std::vector<RenderCommand> opaqueRenderQueue; ///< Opaque render queue.
        std::vector<RenderCommand> transparentRenderQueue; ///< Transparent render queue.
    };

    /**
     * @brief Structure containing renderer statistics.
     */
    struct Renderer3DStats
    {
        uint32_t DrawCalls = 0; ///< Number of draw calls.
        uint32_t VertexCount = 0; ///< Number of vertices.
        uint32_t IndexCount = 0; ///< Number of indices.

        void Reset()
        {
            DrawCalls = 0;
            VertexCount = 0;
            IndexCount = 0;
        }
    };

    /**
     * @brief Structure containing render settings.
     */
    struct Renderer3DSettings
    {
        bool SSAO = false; ///< Enable or disable SSAO.

        bool DepthFog = false; ///< Enable or disable depth fog.
        glm::vec3 FogColor = {0.5f, 0.5f, 0.5f}; ///< Fog color.
        float FogDensity = 0.1f; ///< Fog density.
        float FogHeight = 0.0f; ///< Fog height.
        float FogHeightDensity = 0.0f; ///< Fog height density.

        bool Bloom = false; ///< Enable or disable bloom.
        float BloomIntensity = 0.1f; ///< Bloom intensity.
        float BloomRadius = 1.0f; ///< Bloom radius.
        int BloomMaxMipLevels = 5; ///< Maximum number of mip levels for bloom.

        bool FXAA = true; ///< Enable or disable FXAA.

        float Exposure = 1.0f; ///< Exposure value.
        float EnvironmentExposure = 1.0f; ///< Environment exposure value.

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
        Renderer3D(RendererAPI* api);

        /**
         * @brief Shuts down the renderer.
         */
        void Shutdown();

        void Submit(const RenderCommand& command);

        void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f), uint32_t entityID = 4294967295);

        /**
         * @brief Submits a light component.
         * @param light The light component.
         */

         //Todo change this to a light class and not a component
        void Submit(const LightComponent& light);

        void SetEnvironmentMap(const Ref<Cubemap>& environmentMap) { m_RendererData.EnvironmentMap = environmentMap; }
        
        void DepthPrePass(const Ref<RenderTarget>& target);
        //void SSAOPass(const Ref<RenderTarget>& target);
        void ShadowPass(const Ref<RenderTarget>& target);
        void ForwardPass(const Ref<RenderTarget>& target);
        void SkyboxPass(const Ref<RenderTarget>& target);
        void TransparentPass(const Ref<RenderTarget>& target);
        void PostProcessingPass(const Ref<RenderTarget>& target);

        /**
         * @brief Gets the renderer data.
         * @return A reference to the renderer data.
         */
        const Renderer3DData& GetData() { return m_RendererData; }

        /**
         * @brief Gets the renderer statistics.
         * @return A reference to the renderer statistics.
         */
        const Renderer3DStats& GetStats() { return m_Stats; }
        void ResetStats() { m_Stats.Reset(); }

        /**
         * @brief Gets the render settings.
         * @return A reference to the render settings.
         */
        Renderer3DSettings& GetRenderSettings() { return m_RenderSettings; }
        
        // TODO: Think better name for this
        /*
            - Reset Lights Count
            - Reset Render Queue
        */
        void ResetCalls();
    
    private:
        void GenerateBRDFLUT();

    private:
        RendererAPI* m_API; ///< The renderer API used by the renderer.

        Renderer3DData m_RendererData; ///< Renderer data.
        Renderer3DStats m_Stats; ///< Renderer statistics.
        Renderer3DSettings m_RenderSettings; ///< Render settings.

        Ref<Mesh> m_ScreenQuad; ///< Screen quad mesh.
        Ref<Mesh> m_CubeMesh; ///< Cube mesh.

        Ref<Shader> m_FogShader; ///< Fog shader.
        Ref<Shader> m_ToneMappingShader; ///< Tone mapping shader.
        Ref<Shader> m_FXAAShader; ///< Fast Approximate AntiAliasing shader
        Ref<Shader> m_FinalPassShader; ///< Final pass shader.
        Ref<Shader> m_SkyboxShader; ///< Skybox shader.
        Ref<Shader> depthShader; ///< Depth shader.
        Ref<Shader> brdfShader; ///< BRDF shader.
        Ref<Shader> m_BloomShader; ///< Bloom downsample shader.

        Ref<Framebuffer> m_BloomFramebuffer; ///< Bloom framebuffer.
        Ref<Texture2D> m_BloomDownsampleTexture; ///< Bloom downsampled texture.
        Ref<Texture2D> m_BloomUpsampleTexture; ///< Bloom upsampled texture.
    };

    /** @} */
}