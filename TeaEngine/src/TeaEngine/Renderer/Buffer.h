#pragma once

#include "TeaEngine/Core/Base.h"
#include <cstdint>

namespace Tea {
    
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