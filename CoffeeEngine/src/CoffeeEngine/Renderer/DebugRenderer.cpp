#include "DebugRenderer.h"
#include "CoffeeEngine/Math/Frustum.h"
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

    constexpr size_t MaxVertices = 100000;
    DebugVertex DebugRenderer::m_LineVertices[MaxVertices];
    DebugVertex DebugRenderer::m_CircleVertices[MaxVertices];
    int DebugRenderer::m_LineVertexCount = 0;
    int DebugRenderer::m_CircleVertexCount = 0;

    void DebugRenderer::Init()
    {
        m_DebugShader = CreateRef<Shader>("DebugLineShader", std::string(debugLineShaderSource));

        BufferLayout DebugVertexLayout = {
            {ShaderDataType::Vec3, "a_Position"},
            {ShaderDataType::Vec4, "a_Color"}
        };

        m_LineVertexArray = VertexArray::Create();
        m_LineVertexBuffer = VertexBuffer::Create(MaxVertices * sizeof(DebugVertex));
        m_LineVertexBuffer->SetLayout(DebugVertexLayout);
        m_LineVertexArray->AddVertexBuffer(m_LineVertexBuffer);

        m_CircleVertexArray = VertexArray::Create();
        m_CircleVertexBuffer = VertexBuffer::Create(MaxVertices * sizeof(DebugVertex));
        m_CircleVertexBuffer->SetLayout(DebugVertexLayout);
        m_CircleVertexArray->AddVertexBuffer(m_CircleVertexBuffer);

        //m_Framebuffer = Framebuffer::Create(1280, 720, {ImageFormat::RGBA8});
        //m_RenderTexture = m_Framebuffer->GetColorTexture(0);
    }

    void DebugRenderer::Shutdown()
    {
    }

    void DebugRenderer::Flush()
    {
        // Get the current Framebuffer and store it
        // Bind the framebuffer to render the debug lines
        // Restore the previous framebuffer

        if (m_LineVertexCount > 0)
        {
            m_LineVertexBuffer->SetData(m_LineVertices, m_LineVertexCount * sizeof(DebugVertex));
            m_DebugShader->Bind();
            RendererAPI::DrawLines(m_LineVertexArray, m_LineVertexCount, 1.0f);
            m_LineVertexCount = 0;
        }

        if (m_CircleVertexCount > 0)
        {
            m_CircleVertexBuffer->SetData(m_CircleVertices, m_CircleVertexCount * sizeof(DebugVertex));
            m_DebugShader->Bind();
            RendererAPI::DrawLines(m_CircleVertexArray, m_CircleVertexCount, 1.0f);
            m_CircleVertexCount = 0;
        }
    }

    void DebugRenderer::DrawLine(const glm::vec3& start, const glm::vec3& end, glm::vec4 color, float lineWidth)
    {
        if (m_LineVertexCount + 2 <= MaxVertices)
        {
            m_LineVertices[m_LineVertexCount++] = {start, color};
            m_LineVertices[m_LineVertexCount++] = {end, color};
        }
    }

    void DebugRenderer::DrawCircle(const glm::vec3& position, float radius, const glm::quat& rotation, glm::vec4 color, float lineWidth)
    {
        const int segments = 32;
        const float angleStep = 2.0f * 3.14159f / segments;

        for(int i = 0; i < segments; i++)
        {
            if (m_CircleVertexCount + 2 > MaxVertices)
                break;

            float cx = cos(i * angleStep) * radius;
            float cy = sin(i * angleStep) * radius;
            glm::vec3 p0 = position + glm::toMat3(rotation) * glm::vec3(cx, cy, 0.0f);

            cx = cos((i + 1) * angleStep) * radius;
            cy = sin((i + 1) * angleStep) * radius;
            glm::vec3 p1 = position + glm::toMat3(rotation) * glm::vec3(cx, cy, 0.0f);

            m_CircleVertices[m_CircleVertexCount++] = {p0, color};
            m_CircleVertices[m_CircleVertexCount++] = {p1, color};
        }
    }

    void DebugRenderer::DrawSphere(const glm::vec3& position, float radius, const glm::vec4& color, float lineWidth)
    {
        DrawCircle(position, radius, glm::quat(), color, lineWidth);
        DrawCircle(position, radius, glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)), color, lineWidth);
        DrawCircle(position, radius, glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), color, lineWidth);
    }

    void DebugRenderer::DrawBox(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& size, const glm::vec4& color, const bool& isCentered, float lineWidth)
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
                if (m_CircleVertexCount + 2 > MaxVertices)
                    break;

                glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::pi<float>() * i / arrow_sides, glm::vec3(0, 0, 1));

                glm::vec3 v1 = arrow[j] - glm::vec3(0, 0, arrow_length);
                glm::vec3 v2 = arrow[(j + 1) % arrow_points] - glm::vec3(0, 0, arrow_length);

                glm::vec3 transformed_v1 = glm::vec3(transform * rotation * glm::vec4(v1, 1.0f));
                glm::vec3 transformed_v2 = glm::vec3(transform * rotation * glm::vec4(v2, 1.0f));

                m_CircleVertices[m_CircleVertexCount++] = {transformed_v1, color};
                m_CircleVertices[m_CircleVertexCount++] = {transformed_v2, color};
            }
        }
    }

    void DebugRenderer::DrawArrow(const glm::vec3& origin, const glm::vec3& direction, float length, glm::vec4 color, float lineWidth)
    {
        glm::vec3 end = origin - direction * length;
        DrawArrow(origin, end, false, color, lineWidth);
    }

    void DebugRenderer::DrawFrustum(const Frustum& frustum, const glm::vec4& color, float lineWidth)
    {
        const glm::vec3* points = frustum.GetPoints();
        
        // Draw near plane rectangle
        DrawLine(points[0], points[1], color, lineWidth);
        DrawLine(points[0], points[2], color, lineWidth);
        DrawLine(points[2], points[3], color, lineWidth);
        DrawLine(points[1], points[3], color, lineWidth);
        
        // Draw far plane rectangle
        DrawLine(points[4], points[5], color, lineWidth);
        DrawLine(points[4], points[6], color, lineWidth);
        DrawLine(points[6], points[7], color, lineWidth);
        DrawLine(points[5], points[7], color, lineWidth);
        
        // Draw connecting lines between near and far planes
        DrawLine(points[0], points[4], color, lineWidth);
        DrawLine(points[1], points[5], color, lineWidth);
        DrawLine(points[2], points[6], color, lineWidth);
        DrawLine(points[3], points[7], color, lineWidth);
    }

    void DebugRenderer::DrawFrustum(const glm::mat4& viewProjection, const glm::vec4& color, float lineWidth)
    {
        glm::mat4 invVP = glm::inverse(viewProjection);

        glm::vec3 frustumCorners[8] = {
            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3(1.0f, -1.0f, -1.0f),
            glm::vec3(1.0f, 1.0f, -1.0f),
            glm::vec3(-1.0f, 1.0f, -1.0f),
            glm::vec3(-1.0f, -1.0f, 1.0f),
            glm::vec3(1.0f, -1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(-1.0f, 1.0f, 1.0f)
        };

        for (int i = 0; i < 8; i++) {
            glm::vec4 clipSpace = invVP * glm::vec4(frustumCorners[i], 1.0f);
            clipSpace /= clipSpace.w;
            frustumCorners[i] = clipSpace;
        }

        DrawLine(frustumCorners[0], frustumCorners[1], color, lineWidth);
        DrawLine(frustumCorners[1], frustumCorners[2], color, lineWidth);
        DrawLine(frustumCorners[2], frustumCorners[3], color, lineWidth);
        DrawLine(frustumCorners[3], frustumCorners[0], color, lineWidth);

        DrawLine(frustumCorners[4], frustumCorners[5], color, lineWidth);
        DrawLine(frustumCorners[5], frustumCorners[6], color, lineWidth);
        DrawLine(frustumCorners[6], frustumCorners[7], color, lineWidth);
        DrawLine(frustumCorners[7], frustumCorners[4], color, lineWidth);

        DrawLine(frustumCorners[0], frustumCorners[4], color, lineWidth);
        DrawLine(frustumCorners[1], frustumCorners[5], color, lineWidth);
        DrawLine(frustumCorners[2], frustumCorners[6], color, lineWidth);
        DrawLine(frustumCorners[3], frustumCorners[7], color, lineWidth);
    }
}