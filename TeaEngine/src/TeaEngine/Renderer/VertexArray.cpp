#include "TeaEngine/Renderer/VertexArray.h"

#include <glad/glad.h>

namespace Tea {
    
    VertexArray::VertexArray()
    {
        glCreateVertexArrays(1, &m_vaoID);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_vaoID);
    }

    void VertexArray::Bind()
    {
        glBindVertexArray(m_vaoID);
    }

    void VertexArray::Unbind()
    {
        glBindVertexArray(0);
    }

    void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        glBindVertexArray(m_vaoID);
        vertexBuffer->Bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);

        m_VertexBuffers.push_back(vertexBuffer);
    }

    void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_vaoID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }

    Ref<VertexArray> VertexArray::Create()
    {
        return CreateRef<VertexArray>();
    }

}