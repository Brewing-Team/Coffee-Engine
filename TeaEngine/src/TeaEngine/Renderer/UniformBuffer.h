#pragma once

#include "TeaEngine/Core/Base.h"
#include <cstdint>

namespace Tea {

    /**
     * @defgroup renderer Renderer
     * @brief Renderer components of the TeaEngine.
     * @{
     */

    /**
     * @brief Class representing a uniform buffer.
     */
    class UniformBuffer
    {
    public:
        /**
         * @brief Constructs a UniformBuffer with the specified size and binding.
         * @param size The size of the buffer.
         * @param binding The binding point of the buffer.
         */
        UniformBuffer(uint32_t size, uint32_t binding);

        /**
         * @brief Destructor for the UniformBuffer class.
         */
        virtual ~UniformBuffer();

        /**
         * @brief Sets the data of the uniform buffer.
         * @param data A pointer to the data to set.
         * @param size The size of the data.
         * @param offset The offset in the buffer to set the data.
         */
        void SetData(const void* data, uint32_t size, uint32_t offset = 0);

        /**
         * @brief Creates a uniform buffer with the specified size and binding.
         * @param size The size of the buffer.
         * @param binding The binding point of the buffer.
         * @return A reference to the created uniform buffer.
         */
        static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
    private:
        uint32_t m_uboID; ///< The ID of the uniform buffer.
    };

    /** @} */
}