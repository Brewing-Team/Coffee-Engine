#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/Renderer/Image.h"

#include <cstdint>
#include <string>
#include <unordered_map>

namespace Coffee {

    /**
     * @defgroup renderer Renderer
     * @brief Renderer components of the CoffeeEngine.
     * @{
     */

    /**
     * @brief Enum class representing different image formats.
     */
    enum class ImageFormat
    {
        R8, ///< 8-bit Red channel.
        RG8, ///< 8-bit Red and Green channels.
        RGB8, ///< 8-bit Red, Green, and Blue channels.
        SRGB8, ///< 8-bit sRGB Red, Green, and Blue channels.
        RGBA8, ///< 8-bit Red, Green, Blue, and Alpha channels.
        SRGBA8, ///< 8-bit sRGB Red, Green, Blue, and Alpha channels.
        RGBA32F, ///< 32-bit floating point Red, Green, Blue, and Alpha channels.

        DEPTH24STENCIL8 ///< 24-bit Depth and 8-bit Stencil channels.
    };

    /**
     * @brief Structure representing properties of a texture.
     */
    struct TextureProperties
    {
        ImageFormat Format; ///< The format of the image.
        uint32_t Width, Height; ///< The width and height of the texture.
        bool GenerateMipmaps = true; ///< Whether to generate mipmaps.
        bool srgb = true; ///< Whether the texture is in sRGB format.
    };
    /**
     * @brief Class representing a texture.
     */
    class Texture : public Resource
    {
    public:
        /**
         * @brief Constructs a Texture with the specified properties.
         * @param properties The properties of the texture.
         */
        Texture(const TextureProperties& properties);

        /**
         * @brief Constructs a Texture with the specified width, height, and image format.
         * @param width The width of the texture.
         * @param height The height of the texture.
         * @param imageFormat The format of the image.
         */
        Texture(uint32_t width, uint32_t height, ImageFormat imageFormat);

        /**
         * @brief Constructs a Texture from a file path.
         * @param path The file path to the texture.
         * @param srgb Whether the texture is in sRGB format.
         */
        Texture(const std::filesystem::path& path, bool srgb = true);

        /**
         * @brief Destructor for the Texture class.
         */
        ~Texture();

        /**
         * @brief Binds the texture to the specified slot.
         * @param slot The slot to bind the texture to.
         */
        void Bind(uint32_t slot);

        /**
         * @brief Resizes the texture to the specified width and height.
         * @param width The new width of the texture.
         * @param height The new height of the texture.
         */
        void Resize(uint32_t width, uint32_t height);

        /**
         * @brief Gets the size of the texture.
         * @return A pair containing the width and height of the texture.
         */
        std::pair<uint32_t, uint32_t> GetSize() { return std::make_pair(m_Width, m_Height); };

        /**
         * @brief Gets the width of the texture.
         * @return The width of the texture.
         */
        uint32_t GetWidth() { return m_Width; };

        /**
         * @brief Gets the height of the texture.
         * @return The height of the texture.
         */
        uint32_t GetHeight() { return m_Width; };

        /**
         * @brief Gets the ID of the texture.
         * @return The ID of the texture.
         */
        uint32_t GetID() { return m_textureID; };

        void SetData(void* data, uint32_t size);

        /**
         * @brief Gets the image format of the texture.
         * @return The image format of the texture.
         */
        ImageFormat GetImageFormat() { return m_Properties.Format; };

        /**
         * @brief Loads a texture from a file path.
         * @param path The file path to the texture.
         * @param srgb Whether the texture is in sRGB format.
         * @return A reference to the loaded texture.
         */
        static Ref<Texture> Load(const std::filesystem::path& path, bool srgb = true);

        /**
         * @brief Creates a texture with the specified width, height, and format.
         * @param width The width of the texture.
         * @param height The height of the texture.
         * @param format The format of the texture.
         * @return A reference to the created texture.
         */
        static Ref<Texture> Create(uint32_t width, uint32_t height, ImageFormat format);
    private:
        TextureProperties m_Properties; ///< The properties of the texture.

        uint32_t m_textureID;
        int m_Width, m_Height;
    };
}
