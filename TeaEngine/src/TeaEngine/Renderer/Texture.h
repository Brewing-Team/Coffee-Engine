#pragma once

#include "TeaEngine/Core/Assert.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Image.h"

#include <cstdint>
#include <string>
#include <unordered_map>

namespace Tea {

    /**
     * @defgroup renderer Renderer
     * @brief Renderer components of the TeaEngine.
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
    class Texture
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
        Texture(const std::string& path, bool srgb = true);

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

        /**
         * @brief Gets the file path of the texture.
         * @return The file path of the texture.
         */
        const std::string& GetPath() { TEA_CORE_ASSERT(m_FilePath.empty(), "This Texture does not exist on disk!") return m_FilePath; };

        /**
         * @brief Sets the data of the texture.
         * @param data The data to set.
         * @param size The size of the data.
         */
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
        static Ref<Texture> Load(const std::string& path, bool srgb = true);

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

        std::string m_FilePath; ///< The file path of the texture.
        uint32_t m_textureID; ///< The ID of the texture.
        int m_Width, m_Height; ///< The width and height of the texture.
    };

    /**
     * @brief Class representing a library of textures.
     */
    class TextureLibrary
    {
    public:
        /**
         * @brief Adds a texture to the library.
         * @param name The name of the texture.
         * @param texture A reference to the texture.
         */
        static void Add(const std::string& name, const Ref<Texture>& texture) { m_Textures[name] = texture; }

        /**
         * @brief Gets a texture from the library by name.
         * @param name The name of the texture.
         * @return A reference to the texture.
         */
        static Ref<Texture> Get(const std::string& name) { return m_Textures[name]; }

        /**
         * @brief Checks if a texture exists in the library by name.
         * @param name The name of the texture.
         * @return True if the texture exists, false otherwise.
         */
        static bool Exists(const std::string& name) { return m_Textures[name] ? true : false; }
    private:
        static std::unordered_map<std::string, Ref<Texture>> m_Textures; ///< The map of textures.
    };

    /** @} */
}