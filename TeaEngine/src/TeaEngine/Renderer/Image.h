#pragma once

#include "TeaEngine/Core/Base.h"
#include <string>
#include <utility>

namespace Tea {

    enum class ImageChannels
    {
        R8,
        RG8,
        RGB8,
        RGBA8
    };

    class Image
    {
    public:
        Image(std::string& path);

        static Ref<Image> Load(std::string& path); //Idk if should be a Ref or a Scope

        unsigned char* GetData() { return m_Data; };
        std::pair<int, int> GetSize() { return std::make_pair(m_Width, m_Height); };
        const std::string& GetPath() { return m_FilePath; };
        ImageChannels GetChannels() { return m_Channels; };

    private:
        std::string m_FilePath;
        unsigned char* m_Data;
        int m_Width, m_Height;
        ImageChannels m_Channels;
    };

}