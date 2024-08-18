#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Renderer/Mesh.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include "TeaEngine/Renderer/Shader.h"
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
    };

    struct RendererStats
    {
        uint32_t DrawCalls = 0;
        uint32_t VertexCount = 0;
        uint32_t IndexCount = 0;
    };

    class Renderer //The 3d Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(EditorCamera& camera);
        static void EndScene();

        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
        static void Submit(const Ref<Material>& material, const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));
    private:

        static RendererData s_RendererData;
        static RendererStats s_Stats;

    };

}