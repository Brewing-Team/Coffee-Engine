#pragma once

#include "CoffeeEngine/Core/Base.h"

#include <filesystem>

namespace Coffee {

    class Texture2D;
    struct MSDFData;

    class Font
    {
    public:
        Font(const std::filesystem::path& path);
        ~Font();

        const MSDFData* GetMSDFData() const { return m_Data; }
        Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

        static Ref<Font> GetDefault();
    private:
        MSDFData* m_Data;
        Ref<Texture2D> m_AtlasTexture;
    };

}