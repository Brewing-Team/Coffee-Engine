#include "DebugRenderer.h"
#include "CoffeeEngine/Renderer/Buffer.h"
#include "CoffeeEngine/Renderer/RendererAPI.h"
#include "CoffeeEngine/Renderer/VertexArray.h"

#include "CoffeeEngine/Embedded/DebugLineShader.inl"

#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/fwd.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "Camera.h"

#include <glm/gtx/quaternion.hpp>

namespace Coffee {

    Ref<VertexArray> DebugRenderer::m_LineVertexArray;
    Ref<VertexBuffer> DebugRenderer::m_LineVertexBuffer;

    Ref<VertexArray> DebugRenderer::m_CircleVertexArray;
    Ref<VertexBuffer> DebugRenderer::m_CircleVertexBuffer;

    Ref<Shader> DebugRenderer::m_DebugShader;

    void DebugRenderer::Init()
    {
        m_DebugShader = CreateRef<Shader>("DebugLineShader", std::string(debugLineShaderSource));

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

    void DebugRenderer::DrawSphere(const glm::vec3& position, float radius, const glm::vec4& color, float lineWidth)
    {
        DrawCircle(position, radius, glm::quat(), color, lineWidth);
        DrawCircle(position, radius, glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)), color,
                   lineWidth);
        DrawCircle(position, radius, glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), color,
                   lineWidth);
    }

    // TODO fix this function, currently is not drawing the bounding box at all
    void DebugRenderer::DrawBox(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& size,
                                const glm::vec4& color, const bool& isCentered, float lineWidth)
    {
        glm::vec3 halfSize = size * 0.5f;
        glm::vec3 vertices[8];

        if (isCentered) {
            vertices[0] = position + rotation * glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z);
            vertices[1] = position + rotation * glm::vec3(halfSize.x, -halfSize.y, -halfSize.z);
            vertices[2] = position + rotation * glm::vec3(halfSize.x, halfSize.y, -halfSize.z);
            vertices[3] = position + rotation * glm::vec3(-halfSize.x, halfSize.y, -halfSize.z);
            vertices[4] = position + rotation * glm::vec3(-halfSize.x, -halfSize.y, halfSize.z);
            vertices[5] = position + rotation * glm::vec3(halfSize.x, -halfSize.y, halfSize.z);
            vertices[6] = position + rotation * glm::vec3(halfSize.x, halfSize.y, halfSize.z);
            vertices[7] = position + rotation * glm::vec3(-halfSize.x, halfSize.y, halfSize.z);
        } else {
            vertices[0] = position + rotation * glm::vec3(0.0f, 0.0f, 0.0f);
            vertices[1] = position + rotation * glm::vec3(size.x, 0.0f, 0.0f);
            vertices[2] = position + rotation * glm::vec3(size.x, size.y, 0.0f);
            vertices[3] = position + rotation * glm::vec3(0.0f, size.y, 0.0f);
            vertices[4] = position + rotation * glm::vec3(0.0f, 0.0f, size.z);
            vertices[5] = position + rotation * glm::vec3(size.x, 0.0f, size.z);
            vertices[6] = position + rotation * glm::vec3(size.x, size.y, size.z);
            vertices[7] = position + rotation * glm::vec3(0.0f, size.y, size.z);
        }

        DrawLine(vertices[0], vertices[1], color, lineWidth);
        DrawLine(vertices[1], vertices[2], color, lineWidth);
        DrawLine(vertices[2], vertices[3], color, lineWidth);
        DrawLine(vertices[3], vertices[0], color, lineWidth);

        DrawLine(vertices[4], vertices[5], color, lineWidth);
        DrawLine(vertices[5], vertices[6], color, lineWidth);
        DrawLine(vertices[6], vertices[7], color, lineWidth);
        DrawLine(vertices[7], vertices[4], color, lineWidth);

        DrawLine(vertices[0], vertices[4], color, lineWidth);
        DrawLine(vertices[1], vertices[5], color, lineWidth);
        DrawLine(vertices[2], vertices[6], color, lineWidth);
        DrawLine(vertices[3], vertices[7], color, lineWidth);
    }

    void DebugRenderer::DrawBox(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color, float lineWidth)
    {
        DrawLine(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), color, lineWidth);
        DrawLine(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z), color, lineWidth);
        DrawLine(glm::vec3(max.x, max.y, min.z), glm::vec3(min.x, max.y, min.z), color, lineWidth);
        DrawLine(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, min.y, min.z), color, lineWidth);

        DrawLine(glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z), color, lineWidth);
        DrawLine(glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, max.z), color, lineWidth);
        DrawLine(glm::vec3(max.x, max.y, max.z), glm::vec3(min.x, max.y, max.z), color, lineWidth);
        DrawLine(glm::vec3(min.x, max.y, max.z), glm::vec3(min.x, min.y, max.z), color, lineWidth);

        DrawLine(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, min.y, max.z), color, lineWidth);
        DrawLine(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z), color, lineWidth);
        DrawLine(glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z), color, lineWidth);
        DrawLine(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, max.y, max.z), color, lineWidth);

    }

    void DebugRenderer::DrawBox(const AABB& aabb, const glm::vec4& color, float lineWidth)
    {
        DrawBox(aabb.min, aabb.max, color, lineWidth);
    }

    void DebugRenderer::DrawBox(const OBB& obb, const glm::vec4& color, float lineWidth)
    {
        for (int i = 0; i < 4; i++) {
            DrawLine(obb.corners[i], obb.corners[(i + 1) % 4], color, lineWidth);
            DrawLine(obb.corners[i + 4], obb.corners[(i + 1) % 4 + 4], color, lineWidth);
            DrawLine(obb.corners[i], obb.corners[i + 4], color, lineWidth);
        }
    }


    void DebugRenderer::DrawArrow(const glm::vec3& start, const glm::vec3& end, bool fixedLength, glm::vec4 color, float lineWidth)
    {
        //===============ARROW TRANSLATION FROM ARROW GIZMO GODOT=============
/*         const int arrow_points = 7;
        const float arrow_length = 1.5f;

        glm::vec3 arrow[arrow_points] = {
            glm::vec3(0, 0, -1),
            glm::vec3(0, 0.8f, 0),
            glm::vec3(0, 0.3f, 0),
            glm::vec3(0, 0.3f, arrow_length),
            glm::vec3(0, -0.3f, arrow_length),
            glm::vec3(0, -0.3f, 0),
            glm::vec3(0, -0.8f, 0)
        };

        const int arrow_sides = 2;

        std::vector<DebugVertex> vertices;

        for (int i = 0; i < arrow_sides; i++) {
            for (int j = 0; j < arrow_points; j++) {
                glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::pi<float>() * i / arrow_sides, glm::vec3(0, 0, 1));

                glm::vec3 v1 = arrow[j] - glm::vec3(0, 0, arrow_length);
                glm::vec3 v2 = arrow[(j + 1) % arrow_points] - glm::vec3(0, 0, arrow_length);

                glm::vec3 transformed_v1 = glm::vec3(rotation * glm::vec4(v1, 1.0f));
                glm::vec3 transformed_v2 = glm::vec3(rotation * glm::vec4(v2, 1.0f));

                vertices.push_back({transformed_v1, color});
                vertices.push_back({transformed_v2, color});
            }
        } */

        //====================================================================

        const int arrow_points = 7;
        const float arrow_length = fixedLength ? 1.5f : glm::length(end - start);

        glm::vec3 arrow[arrow_points] = {
            glm::vec3(0, 0, -1),
            glm::vec3(0, 0.8f, 0),
            glm::vec3(0, 0.3f, 0),
            glm::vec3(0, 0.3f, arrow_length),
            glm::vec3(0, -0.3f, arrow_length),
            glm::vec3(0, -0.3f, 0),
            glm::vec3(0, -0.8f, 0)
        };

        const int arrow_sides = 2;

        std::vector<DebugVertex> vertices;

        glm::vec3 direction = glm::normalize(end - start);
        glm::vec3 up = glm::vec3(0, 1, 0);
        if (glm::abs(glm::dot(direction, up)) > 0.99f) {
            up = glm::vec3(1, 0, 0);
        }
        glm::vec3 right = glm::normalize(glm::cross(up, direction));
        up = glm::cross(direction, right);

        glm::mat4 transform = glm::mat4(1.0f);
        transform[0] = glm::vec4(right, 0.0f);
        transform[1] = glm::vec4(up, 0.0f);
        transform[2] = glm::vec4(direction, 0.0f);
        transform[3] = glm::vec4(start, 1.0f);

        for (int i = 0; i < arrow_sides; i++) {
            for (int j = 0; j < arrow_points; j++) {
                glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::pi<float>() * i / arrow_sides, glm::vec3(0, 0, 1));

                glm::vec3 v1 = arrow[j] - glm::vec3(0, 0, arrow_length);
                glm::vec3 v2 = arrow[(j + 1) % arrow_points] - glm::vec3(0, 0, arrow_length);

                glm::vec3 transformed_v1 = glm::vec3(transform * rotation * glm::vec4(v1, 1.0f));
                glm::vec3 transformed_v2 = glm::vec3(transform * rotation * glm::vec4(v2, 1.0f));

                vertices.push_back({transformed_v1, color});
                vertices.push_back({transformed_v2, color});
            }
        }

        m_CircleVertexBuffer->SetData(vertices.data(), vertices.size() * sizeof(DebugVertex));

        m_DebugShader->Bind();

        RendererAPI::DrawLines(m_CircleVertexArray, vertices.size(), lineWidth);
    }

    void DebugRenderer::DrawArrow(const glm::vec3& origin, const glm::vec3& direction, float length, glm::vec4 color, float lineWidth)
    {
        glm::vec3 end = origin - direction * length;
        DrawArrow(origin, end, false, color, lineWidth);
    }
}