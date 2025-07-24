#include "Buffer.h"

#include <glad/glad.h>

#include <tracy/Tracy.hpp>

namespace Coffee {

    VertexBuffer::VertexBuffer(uint32_t size)
    {
        ZoneScoped;

        glGenBuffers(1, &m_vboID);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
    {
        ZoneScoped;

        glGenBuffers(1, &m_vboID);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        ZoneScoped;

        glDeleteBuffers(1, &m_vboID);
    }

    void VertexBuffer::Bind()
    {
        ZoneScoped;

        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    }

    void VertexBuffer::Unbind()
    {
        ZoneScoped;

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::SetData(void* data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        return CreateRef<VertexBuffer>(size);
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        return CreateRef<VertexBuffer>(vertices, size);
    }

    IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
    {
        ZoneScoped;

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
        ZoneScoped;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
    }

    void IndexBuffer::Unbind()
    {
        ZoneScoped;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t count)
    {
        return CreateRef<IndexBuffer>(indices, count);
    }

}
