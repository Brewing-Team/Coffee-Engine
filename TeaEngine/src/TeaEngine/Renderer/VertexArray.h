#pragma once

#include <TeaEngine/Renderer/Buffer.h>

#include <cstdint>
#include <vector>

namespace Tea {

    class VertexArray
    {
    public:

        VertexArray();
        virtual ~VertexArray();

        void Bind();
        void Unbind();

        void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
        void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

        const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return  m_VertexBuffers; }
        const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

        static Ref<VertexArray> Create();
    private:
        uint32_t m_vaoID;
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;
    };
}
