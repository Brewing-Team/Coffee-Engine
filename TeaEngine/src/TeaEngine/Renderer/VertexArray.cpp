#include "TeaEngine/Renderer/VertexArray.h"

#include <glad/glad.h>

namespace Tea {

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
            case ShaderDataType::Bool:     return GL_BOOL;
            case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Vec2:     return GL_FLOAT;
			case ShaderDataType::Vec3:     return GL_FLOAT;
			case ShaderDataType::Vec4:     return GL_FLOAT;
            case ShaderDataType::Mat2:     return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
		}

		TEA_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}
    
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
		TEA_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_vaoID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& attribute : layout)
		{
			switch (attribute.Type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Vec2:
				case ShaderDataType::Vec3:
				case ShaderDataType::Vec4:
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex,
						attribute.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(attribute.Type),
						attribute.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)attribute.Offset);
					m_VertexBufferIndex++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribIPointer(m_VertexBufferIndex,
						attribute.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(attribute.Type),
						layout.GetStride(),
						(const void*)attribute.Offset);
					m_VertexBufferIndex++;
					break;
				}
                case ShaderDataType::Mat2:
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					uint8_t count = attribute.GetComponentCount();
					for (uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(m_VertexBufferIndex);
						glVertexAttribPointer(m_VertexBufferIndex,
							count,
							ShaderDataTypeToOpenGLBaseType(attribute.Type),
							attribute.Normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(attribute.Offset + sizeof(float) * count * i));
						glVertexAttribDivisor(m_VertexBufferIndex, 1);
						m_VertexBufferIndex++;
					}
					break;
				}
				default:
					TEA_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}

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
