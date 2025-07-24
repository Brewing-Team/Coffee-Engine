#pragma once

#include "CoffeeEngine/Core/Assert.h"
#include "CoffeeEngine/Core/Base.h"

#include <stdint.h>
#include <vector>

namespace Coffee {

    /**
     * @defgroup renderer Renderer
     * @brief Renderer components of the CoffeeEngine.
     * @{
     */

    /**
     * @brief Enum class representing different shader data types.
     */
    enum class ShaderDataType
    {
        None = 0, Bool, Int, Float, Vec2, Vec3, Vec4, IVec4, Mat2, Mat3, Mat4
    };

    /**
     * @brief Returns the size of the specified ShaderDataType.
     * @param type The ShaderDataType.
     * @return The size of the ShaderDataType in bytes.
     */
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
            case ShaderDataType::IVec4:    return 4 * 4;
            case ShaderDataType::Mat2:     return 4 * 2 * 2;
            case ShaderDataType::Mat3:     return 4 * 3 * 3;
            case ShaderDataType::Mat4:     return 4 * 4 * 4;
        }

        COFFEE_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    /**
     * @brief Structure representing a buffer attribute.
     */
    struct BufferAttribute
    {
        std::string Name; ///< The name of the attribute.
        ShaderDataType Type; ///< The type of the attribute.
        uint32_t Size; ///< The size of the attribute.
        size_t Offset; ///< The offset of the attribute.
        bool Normalized; ///< Whether the attribute is normalized.

        /**
         * @brief Default constructor for BufferAttribute.
         */
        BufferAttribute() = default;

        /**
         * @brief Constructs a BufferAttribute with the specified parameters.
         * @param type The type of the attribute.
         * @param name The name of the attribute.
         * @param normalized Whether the attribute is normalized.
         */
        BufferAttribute(ShaderDataType type, const std::string& name, bool normalized = false)
            : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
        {
        }

        /**
         * @brief Returns the number of components in the attribute.
         * @return The number of components.
         */
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
                case ShaderDataType::IVec4:   return 4;
                case ShaderDataType::Mat2:    return 2;
                case ShaderDataType::Mat3:    return 3; // 3* float3
                case ShaderDataType::Mat4:    return 4; // 4* float4
            }

            COFFEE_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
        }
    };

    /**
     * @brief Class representing a buffer layout.
     */
    class BufferLayout
    {
    public:
        /**
         * @brief Default constructor for BufferLayout.
         */
        BufferLayout() {}

        /**
         * @brief Constructs a BufferLayout with the specified attributes.
         * @param elements The list of buffer attributes.
         */
        BufferLayout(std::initializer_list<BufferAttribute> elements)
            : m_Attributes(elements)
        {
            CalculateOffsetsAndStride();
        }

        /**
         * @brief Returns the stride of the buffer layout.
         * @return The stride in bytes.
         */
        uint32_t GetStride() const { return m_Stride; }

        /**
         * @brief Returns the list of buffer attributes.
         * @return The list of buffer attributes.
         */
        const std::vector<BufferAttribute>& GetElements() const { return m_Attributes; }

        /**
         * @brief Returns an iterator to the beginning of the buffer attributes.
         * @return An iterator to the beginning.
         */
        std::vector<BufferAttribute>::iterator begin() { return m_Attributes.begin(); }

        /**
         * @brief Returns an iterator to the end of the buffer attributes.
         * @return An iterator to the end.
         */
        std::vector<BufferAttribute>::iterator end() { return m_Attributes.end(); }

        /**
         * @brief Returns a constant iterator to the beginning of the buffer attributes.
         * @return A constant iterator to the beginning.
         */
        std::vector<BufferAttribute>::const_iterator begin() const { return m_Attributes.begin(); }

        /**
         * @brief Returns a constant iterator to the end of the buffer attributes.
         * @return A constant iterator to the end.
         */
        std::vector<BufferAttribute>::const_iterator end() const { return m_Attributes.end(); }

    private:
        /**
         * @brief Calculates the offsets and stride of the buffer attributes.
         */
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
        std::vector<BufferAttribute> m_Attributes; ///< The list of buffer attributes.
        uint32_t m_Stride = 0; ///< The stride of the buffer layout.
    };

    /**
     * @brief Class representing a vertex buffer.
     */
    class VertexBuffer
    {
    public:
        /**
         * @brief Constructs a VertexBuffer with the specified size.
         * @param size The size of the buffer.
         */
        VertexBuffer(uint32_t size);

        /**
         * @brief Constructs a VertexBuffer with the specified vertices and size.
         * @param vertices The vertex data.
         * @param size The size of the buffer.
         */
        VertexBuffer(float* vertices, uint32_t size);

        /**
         * @brief Destroys the VertexBuffer.
         */
        virtual ~VertexBuffer();

        /**
         * @brief Binds the vertex buffer.
         */
        void Bind();

        /**
         * @brief Unbinds the vertex buffer.
         */
        void Unbind();

        /**
         * @brief Sets the data of the vertex buffer.
         * @param data The data to set.
         * @param size The size of the data.
         */
        void SetData(void* data, uint32_t size);

        /**
         * @brief Returns the layout of the vertex buffer.
         * @return The buffer layout.
         */
        const BufferLayout& GetLayout() const { return m_Layout; }

        /**
         * @brief Sets the layout of the vertex buffer.
         * @param layout The buffer layout.
         */
        void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

        /**
         * @brief Creates a vertex buffer with the specified size.
         * @param size The size of the buffer.
         * @return A reference to the created vertex buffer.
         */
        static Ref<VertexBuffer> Create(uint32_t size);

        /**
         * @brief Creates a vertex buffer with the specified vertices and size.
         * @param vertices The vertex data.
         * @param size The size of the buffer.
         * @return A reference to the created vertex buffer.
         */
        static Ref<VertexBuffer> Create(float* vertices, uint32_t size);

    private:
        uint32_t m_vboID; ///< The ID of the vertex buffer object.
        BufferLayout m_Layout; ///< The layout of the vertex buffer.
    };

    /**
     * @brief Class representing an index buffer.
     */
    class IndexBuffer
    {
    public:
        /**
         * @brief Constructs an IndexBuffer with the specified indices and count.
         * @param indices The index data.
         * @param count The number of indices.
         */
        IndexBuffer(uint32_t* indices, uint32_t count);

        /**
         * @brief Destroys the IndexBuffer.
         */
        virtual ~IndexBuffer();

        /**
         * @brief Binds the index buffer.
         */
        void Bind();

        /**
         * @brief Unbinds the index buffer.
         */
        void Unbind();

        /**
         * @brief Returns the number of indices in the buffer.
         * @return The number of indices.
         */
        uint32_t GetCount() const { return m_Count; }

        /**
         * @brief Creates an index buffer with the specified indices and count.
         * @param indices The index data.
         * @param count The number of indices.
         * @return A reference to the created index buffer.
         */
        static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);

    private:
        uint32_t m_eboID; ///< The ID of the element buffer object.
        uint32_t m_Count; ///< The number of indices in the buffer.
    };

    /** @} */
}