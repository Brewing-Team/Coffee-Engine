#include "TeaEngine/Renderer/Mesh.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Core/Log.h"
#include "TeaEngine/Renderer/VertexArray.h"
#include <tracy/Tracy.hpp>

namespace Tea {

    Mesh::Mesh(const std::vector<uint32_t>& indices, const std::vector<Vertex>& vertices)
    {
        ZoneScoped;

        m_Indices = indices;
        m_Vertices = vertices;

        m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size());
        m_VertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));

        BufferLayout layout = {
            {ShaderDataType::Vec3, "a_Position"},
            {ShaderDataType::Vec2, "a_TexCoords"},
            {ShaderDataType::Vec3, "a_Normals"},
            {ShaderDataType::Vec3, "a_Tangent"},
            {ShaderDataType::Vec3, "a_Bitangent"}
        };

        m_VertexBuffer->SetLayout(layout);

        m_VertexArray = VertexArray::Create();
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    }

}
