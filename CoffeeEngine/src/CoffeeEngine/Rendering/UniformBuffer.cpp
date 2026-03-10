#include "UniformBuffer.h"
#include "CoffeeEngine/Core/Base.h"

#include <stdint.h>
#include <glad/glad.h>

namespace Coffee {

    UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding)
    {
        glCreateBuffers(1, &m_uboID);
        glNamedBufferData(m_uboID, size, nullptr, GL_DYNAMIC_DRAW); //or GL_DYNAMIC_DRAW? Search what are the differences
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_uboID);
    }

    UniformBuffer::~UniformBuffer()
    {
        glDeleteBuffers(1, &m_uboID);
    }

    void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
    {
        glNamedBufferSubData(m_uboID, offset, size, data);
    }

    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
    {
        return CreateRef<UniformBuffer>(size, binding);
    }

}
