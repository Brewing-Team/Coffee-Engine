#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Image.h"

#include <cstdint>
#include <string>

namespace Tea {

    enum class ImageFormat
    {
        R8,
        RG8,
        RGB8,
        RGBA8
    };

    class Texture
    {
    public:
        Texture(std::string& path);
        ~Texture();

        void Bind();
        void Unbind();

         std::pair<int, int> GetSize() { return std::make_pair(m_Width, m_Height); };
        const std::string& GetPath() { return m_FilePath; };
        ImageFormat GetImageFormat() { return m_Format; };

        static Ref<Texture> Load(std::string& path);
    private:
        std::string m_FilePath;
        uint32_t m_textureID;
        int m_Width, m_Height;
        ImageFormat m_Format;
    };

}
