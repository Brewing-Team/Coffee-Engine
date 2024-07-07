#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Image.h"

#include <cstdint>
#include <string>

namespace Tea {

    enum class TextureFormat
    {
        R,
        RGB,
        RGBA
    };

    class Texture
    {
    public:
        Texture(std::string& path);
        Texture(Ref<Image> image);
        ~Texture();

        void Bind();
        void Unbind();

        static Ref<Texture> Load(std::string& path);
    private:
        uint32_t m_textureID;
        int m_Width, m_Height;
        TextureFormat m_Format;
    };

}