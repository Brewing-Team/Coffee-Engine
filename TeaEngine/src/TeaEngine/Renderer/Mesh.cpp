#include "TeaEngine/Renderer/Mesh.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Core/Log.h"

namespace Tea {

    Mesh::Mesh(const std::vector<uint32_t>& indices, const std::vector<Vertex>& vertices)
    {
        m_Indices = indices;
        m_Vertices = vertices;

        m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size());
        m_VertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
    }

}