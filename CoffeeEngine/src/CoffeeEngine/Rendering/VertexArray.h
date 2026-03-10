#pragma once

#include "CoffeeEngine/Core/Base.h"

#include <stdint.h>
#include <vector>

namespace Coffee {

    class VertexBuffer;
    class IndexBuffer;

}

namespace Coffee {

    /**
     * @defgroup renderer Renderer
     * @brief Renderer components of the CoffeeEngine.
     * @{
     */

    /**
     * @brief Class representing a vertex array.
     */
    class VertexArray
    {
    public:
        /**
         * @brief Constructs a VertexArray.
         */
        VertexArray();

        /**
         * @brief Destructor for the VertexArray class.
         */
        virtual ~VertexArray();

        /**
         * @brief Binds the vertex array.
         */
        void Bind();

        /**
         * @brief Unbinds the vertex array.
         */
        void Unbind();

        /**
         * @brief Adds a vertex buffer to the vertex array.
         * @param vertexBuffer A reference to the vertex buffer to add.
         */
        void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);

        /**
         * @brief Sets the index buffer for the vertex array.
         * @param indexBuffer A reference to the index buffer to set.
         */
        void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

        /**
         * @brief Gets the vertex buffers of the vertex array.
         * @return A constant reference to the vector of vertex buffers.
         */
        const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return  m_VertexBuffers; }

        /**
         * @brief Gets the index buffer of the vertex array.
         * @return A constant reference to the index buffer.
         */
        const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

        /**
         * @brief Creates a vertex array.
         * @return A reference to the created vertex array.
         */
        static Ref<VertexArray> Create();
    private:
        uint32_t m_vaoID; ///< The ID of the vertex array.
        uint32_t m_VertexBufferIndex = 0; ///< The index of the vertex buffer.
        std::vector<Ref<VertexBuffer>> m_VertexBuffers; ///< The vector of vertex buffers.
        Ref<IndexBuffer> m_IndexBuffer; ///< The index buffer.
    };

    /** @} */
}