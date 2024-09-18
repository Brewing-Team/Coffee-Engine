#include "DebugRenderer.h"
#include "TeaEngine/Renderer/Buffer.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include "TeaEngine/Renderer/VertexArray.h"
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/fwd.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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

    void DebugRenderer::DrawCircle(const glm::vec3& position, float radius, const glm::quat& rotation, glm::vec4 color, float lineWidth)
    {
        const int segments = 32;
        const float angleStep = 2.0f * 3.14159f / segments;

        DebugVertex vertices[segments * 2];

        for(int i = 0; i < segments; i++)
        {
            float cx = cos(i * angleStep) * radius;
            float cy = sin(i * angleStep) * radius;
            glm::vec3 p0 = position + glm::toMat3(rotation) * glm::vec3(cx, cy, 0.0f);

            vertices[i] = { p0, color };

            cx = cos((i + 1) * angleStep) * radius;
            cy = sin((i + 1) * angleStep) * radius;
            glm::vec3 p1 = position + glm::toMat3(rotation) * glm::vec3(cx, cy, 0.0f);

            vertices[i + segments] = { p1, color };
        }

        m_CircleVertexBuffer->SetData(vertices, sizeof(vertices));

        m_DebugShader->Bind();

        RendererAPI::DrawLines(m_CircleVertexArray, segments * 2, lineWidth);
    }

    void DebugRenderer::DrawSphere(const glm::vec3& position, float radius, glm::vec4 color, float lineWidth)
    {
        DrawCircle(position, radius, glm::quat(), color, lineWidth);
        DrawCircle(position, radius, glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)), color, lineWidth);
        DrawCircle(position, radius, glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), color, lineWidth);
    }

}