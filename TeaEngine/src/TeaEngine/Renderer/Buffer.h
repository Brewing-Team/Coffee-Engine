#pragma once

#include "TeaEngine/Core/Base.h"
#include <cstdint>

namespace Tea {
    
    enum class ShaderDataType
	{
		None = 0,  Bool, Int, Float, Vec2, Vec3, Vec4, Mat2, Mat3, Mat4
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
            case ShaderDataType::Bool:     return 1;
            case ShaderDataType::Int:      return 4;
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Vec2:     return 4 * 2;
			case ShaderDataType::Vec3:     return 4 * 3;
			case ShaderDataType::Vec4:     return 4 * 4;
            case ShaderDataType::Mat2:     return 4 * 2 * 2;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
		}

		TEA_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferAttribute
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferAttribute() = default;

		BufferAttribute(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
                case ShaderDataType::Bool:    return 1;
                case ShaderDataType::Int:     return 1;
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Vec2:    return 2;
				case ShaderDataType::Vec3:    return 3;
				case ShaderDataType::Vec4:    return 4;
                case ShaderDataType::Mat2:    return 2;
				case ShaderDataType::Mat3:    return 3; // 3* float3
				case ShaderDataType::Mat4:    return 4; // 4* float4
			}

			TEA_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(std::initializer_list<BufferAttribute> elements)
			: m_Attributes(elements)
		{
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() const { return m_Stride; }
		const std::vector<BufferAttribute>& GetElements() const { return m_Attributes; }

		std::vector<BufferAttribute>::iterator begin() { return m_Attributes.begin(); }
		std::vector<BufferAttribute>::iterator end() { return m_Attributes.end(); }
		std::vector<BufferAttribute>::const_iterator begin() const { return m_Attributes.begin(); }
		std::vector<BufferAttribute>::const_iterator end() const { return m_Attributes.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			m_Stride = 0;
			for (auto& attribute : m_Attributes)
			{
				attribute.Offset = offset;
				offset += attribute.Size;
				m_Stride += attribute.Size;
			}
		}
	private:
		std::vector<BufferAttribute> m_Attributes;
		uint32_t m_Stride = 0;
	};

    class VertexBuffer
    {
    public:
        VertexBuffer(u_int32_t size);
        VertexBuffer(float* vertices, u_int32_t size);
        virtual ~VertexBuffer();

        void Bind();
        void Unbind();

        void SetData(void* data, uint32_t size);

        const BufferLayout& GetLayout() const { return m_Layout; }
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

        static Ref<VertexBuffer> Create(uint32_t size);
        static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
    private:
        uint32_t m_vboID;
        BufferLayout m_Layout;
    };

    class IndexBuffer
    {
    public:
        IndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~IndexBuffer();

        void Bind();
        void Unbind();

        uint32_t GetCount() const { return m_Count; }

        static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
    private:
    uint32_t m_eboID;
    uint32_t m_Count;
    };

}
