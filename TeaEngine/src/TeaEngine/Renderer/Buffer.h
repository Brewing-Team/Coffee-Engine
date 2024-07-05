#pragma once

#include "TeaEngine/Core/Base.h"
#include <cstdint>

namespace Tea {
    
    enum class ShaderDataType
    {
        None = 0, Bool, Int, Float, Vec2, Vec3, Vec4, Mat2, Mat3, Mat4
    };

    template <ShaderDataType T>
    class BufferAttribute
    {
    public:
        BufferAttribute(std::string& name)
        {
            using enum ShaderDataType;
            switch(T)
            {
                case Bool: BufferAttribute(name, 1, 1); break;
                case Int: BufferAttribute(name, 1, 4); break;
            }
        }
    private:
    BufferAttribute(std::string& name, int components, int size)
        : m_Name(name), m_Components(components), m_Size(size), m_Offset(0) {}
    private:
        std::string m_Name;
        unsigned int m_Components;
        unsigned int m_Size;
        unsigned int m_Offset;
    };

    class BufferLayout
    {
        BufferLayout(std::initializer_list<BufferAttribute<ShaderDataType::None>> atrributes)
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

        static Ref<VertexBuffer> Create(uint32_t size);
        static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
    private:
        uint32_t m_vboID;
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
