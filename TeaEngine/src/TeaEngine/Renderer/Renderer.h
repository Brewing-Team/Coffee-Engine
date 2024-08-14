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

    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(EditorCamera& camera);
        static void EndScene();

        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
        static void Submit(const Ref<Material>& material, const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));
    private:
        static Ref<UniformBuffer> s_CameraUniformBuffer;

    };

}