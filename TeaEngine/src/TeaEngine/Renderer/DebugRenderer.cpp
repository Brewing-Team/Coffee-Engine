#include "DebugRenderer.h"
#include "TeaEngine/Renderer/RendererAPI.h"

namespace Tea {

    Ref<VertexArray> DebugRenderer::m_LineVertexArray;
    Ref<VertexBuffer> DebugRenderer::m_LineVertexBuffer;
    Ref<Shader> DebugRenderer::m_LineShader;

    void DebugRenderer::Init()
    {
        m_LineShader = Shader::Create("assets/shaders/DebugLineShader.vert", "assets/shaders/DebugLineShader.frag");

        m_LineVertexArray = VertexArray::Create();
        //m_LineVertexBuffer = VertexBuffer::Create(nullptr, 2 * sizeof(LineVertex));
        m_LineVertexBuffer = VertexBuffer::Create(2 * sizeof(LineVertex));
        m_LineVertexBuffer->SetLayout({
            {ShaderDataType::Vec3, "a_Position"},
            {ShaderDataType::Vec4, "a_Color"}
        });
        m_LineVertexArray->AddVertexBuffer(m_LineVertexBuffer);
    }

    void DebugRenderer::Shutdown()
    {
    }

    void DebugRenderer::DrawLine(const glm::vec3& start, const glm::vec3& end, glm::vec4 color)
    {
        LineVertex vertices[2] = {
            {start, color},
            {end, color}
        };

        m_LineVertexBuffer->SetData(vertices, sizeof(vertices));

        m_LineShader->Bind();

        RendererAPI::DrawLines(m_LineVertexArray, 2);
    }

}