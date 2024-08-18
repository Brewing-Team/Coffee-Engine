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
#include <glm/fwd.hpp>
namespace Tea {

    struct RendererData
    {
        struct CameraData
        {
            glm::mat4 projection;
            glm::mat4 view;
            glm::vec3 position;
        };

        CameraData cameraData;

        Ref<UniformBuffer> CameraUniformBuffer;

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
        bool PostProcessing = false;

        bool SSAO = false;
        bool Bloom = false;
        bool FXAA = false;
    };

    class Renderer //The 3d Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(EditorCamera& camera);
        static void EndScene();

        static void BeginOverlay(EditorCamera& camera);
        static void EndOverlay();

        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
        static void Submit(const Ref<Material>& material, const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));

        static void OnResize(uint32_t width, uint32_t height);

        static const Ref<Texture>& GetRenderTexture() { return s_RendererData.RenderTexture; }

        static const RendererData& GetData() { return s_RendererData; }
        static const RendererStats& GetStats() { return s_Stats; }
        static const RenderSettings& GetRenderSettings() { return s_RenderSettings; }
    private:

        static RendererData s_RendererData;
        static RendererStats s_Stats;
        static RenderSettings s_RenderSettings;

        //Framebuffers
        static Ref<Framebuffer> s_MainFramebuffer;
    };

}