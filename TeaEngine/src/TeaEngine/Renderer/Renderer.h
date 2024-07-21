#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include "TeaEngine/Renderer/Shader.h"
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
    private:
        static glm::mat4 s_SceneViewMatrix;
        static glm::mat4 s_SceneProjectionMatrix;
    };

}