#include "TeaEngine/Renderer/Buffer.h"

#include <glad/glad.h>

namespace Tea {

    VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
    {
        glGenBuffers(1, &m_vboID);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_vboID);
    }

    void VertexBuffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    }

    void VertexBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, u_int32_t size)
    {
        return CreateRef<VertexBuffer>(vertices, size);
    }

    IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
    {
        glGenBuffers(1, &m_eboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_eboID);
    }

    void IndexBuffer::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
    }

    void IndexBuffer::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t count)
    {
        return CreateRef<IndexBuffer>(indices, count);
    }

}