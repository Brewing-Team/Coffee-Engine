#pragma once

#include "TeaEngine/Renderer/Shader.h"
#include "TeaEngine/Renderer/VertexArray.h"
#include <glm/glm.hpp>
namespace Tea {

    struct LineVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
    };

    class DebugRenderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void DrawLine(const glm::vec3& start, const glm::vec3& end, glm::vec4 color = glm::vec4(1.0f), float lineWidth = 1.0f);
    private:
        static Ref<VertexArray> m_LineVertexArray;
        static Ref<VertexBuffer> m_LineVertexBuffer;
        static Ref<Shader> m_LineShader;
    };

}