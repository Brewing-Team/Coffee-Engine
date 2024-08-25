#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Renderer/Framebuffer.h"
#include "TeaEngine/Renderer/Mesh.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include "TeaEngine/Renderer/Shader.h"
#include "TeaEngine/Renderer/Texture.h"
#include "TeaEngine/Renderer/UniformBuffer.h"
#include "TeaEngine/Renderer/VertexArray.h"
#include "TeaEngine/Scene/Components.h"
#include <glm/fwd.hpp>
#include <vector>
namespace Tea {

    struct RendererData
    {
        struct CameraData
        {
            glm::mat4 projection;
            glm::mat4 view;
            glm::vec3 position;
        };

        struct RenderData
        {
            LightComponent lights[32];

            int lightCount = 0;
        };

        CameraData cameraData;
        RenderData renderData;

        Ref<UniformBuffer> CameraUniformBuffer;
        Ref<UniformBuffer> RenderDataUniformBuffer;

        Ref<Texture> RenderTexture;
    };

    struct RendererStats
    {
        uint32_t DrawCalls = 0;
        uint32_t VertexCount = 0;
        uint32_t IndexCount = 0;
    };

    struct RenderSettings
    {
        bool PostProcessing = true;

        bool SSAO = false;
        bool Bloom = false;
        bool FXAA = false;

        float Exposure = 1.0f;
    };

    class Renderer //The 3d Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(EditorCamera& camera);
        static void BeginScene(const Camera& camera, const glm::mat4& transform);
        static void EndScene();

        static void BeginOverlay(EditorCamera& camera);
        static void EndOverlay();

        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
        static void Submit(const Ref<Material>& material, const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));
        static void Submit(const LightComponent& light);

        static void OnResize(uint32_t width, uint32_t height);

        static const Ref<Texture>& GetRenderTexture() { return s_RendererData.RenderTexture; }

        static const RendererData& GetData() { return s_RendererData; }
        static const RendererStats& GetStats() { return s_Stats; }
        static RenderSettings& GetRenderSettings() { return s_RenderSettings; }
    private:

        static RendererData s_RendererData;
        static RendererStats s_Stats;
        static RenderSettings s_RenderSettings;

        //Textures
        static Ref<Texture> s_MainRenderTexture;
        static Ref<Texture> s_PostProcessingTexture;
        static Ref<Texture> s_DepthTexture;

        //Framebuffers
        static Ref<Framebuffer> s_MainFramebuffer;
        static Ref<Framebuffer> s_PostProcessingFramebuffer;

        static Ref<Mesh> s_ScreenQuad;

        static Ref<Shader> s_ToneMappingShader;
        static Ref<Shader> s_FinalPassShader;
    };

}