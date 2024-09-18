#include "DebugRenderer.h"
#include "TeaEngine/Renderer/Buffer.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include "TeaEngine/Renderer/VertexArray.h"

namespace Tea {

    Ref<VertexArray> DebugRenderer::m_LineVertexArray;
    Ref<VertexBuffer> DebugRenderer::m_LineVertexBuffer;

    Ref<VertexArray> DebugRenderer::m_CircleVertexArray;
    Ref<VertexBuffer> DebugRenderer::m_CircleVertexBuffer;

    Ref<Shader> DebugRenderer::m_DebugShader;

    void DebugRenderer::Init()
    {
        m_DebugShader = Shader::Create("assets/shaders/DebugLineShader.vert", "assets/shaders/DebugLineShader.frag");

        BufferLayout DebugVertexLayout = {
            {ShaderDataType::Vec3, "a_Position"},
            {ShaderDataType::Vec4, "a_Color"}
        };

        m_LineVertexArray = VertexArray::Create();
        m_LineVertexBuffer = VertexBuffer::Create(2 * sizeof(DebugVertex));
        m_LineVertexBuffer->SetLayout(DebugVertexLayout);
        m_LineVertexArray->AddVertexBuffer(m_LineVertexBuffer);

        m_CircleVertexArray = VertexArray::Create();
        m_CircleVertexBuffer = VertexBuffer::Create(64 * sizeof(DebugVertex));
        m_CircleVertexBuffer->SetLayout(DebugVertexLayout);
        m_CircleVertexArray->AddVertexBuffer(m_CircleVertexBuffer);
    }

    void DebugRenderer::Shutdown()
    {
    }

    void DebugRenderer::DrawLine(const glm::vec3& start, const glm::vec3& end, glm::vec4 color, float lineWidth)
    {
        DebugVertex vertices[2] = {
            {start, color},
            {end, color}
        };

        m_LineVertexBuffer->SetData(vertices, sizeof(vertices));

        m_DebugShader->Bind();

        RendererAPI::DrawLines(m_LineVertexArray, 2, lineWidth);
    }

    void DebugRenderer::DrawCircle(const glm::vec3& center, float radius, glm::vec4 color, float lineWidth)
    {
        const int segments = 32;
        const float angleIncrement = 2.0f * 3.14159f / segments;

        DebugVertex vertices[segments * 2];

        for(int i = 0; i < segments; i++)
        {
            float angle = i * angleIncrement;
            vertices[i] = {glm::vec3(center.x + radius * cos(angle), center.y + radius * sin(angle), center.z), color};

            angle = (i + 1) * angleIncrement;
            vertices[i + segments] = {glm::vec3(center.x + radius * cos(angle), center.y + radius * sin(angle), center.z), color};
        }

        m_CircleVertexBuffer->SetData(vertices, sizeof(vertices));

        m_DebugShader->Bind();

        RendererAPI::DrawLines(m_CircleVertexArray, segments * 2, lineWidth);
    }

}