#pragma once 

#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/IO/ResourceImporter.h"
#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include <filesystem>

namespace Coffee {

    class ResourceLoader
    {
    public:
        static void LoadDirectory(const std::filesystem::path& directory);
        static void LoadFile(const std::filesystem::path& path);

        static Ref<Texture> LoadTexture(const std::filesystem::path& path, bool srgb = true, bool cache = true);
        static Ref<Model> LoadModel(const std::filesystem::path& path, bool cache = true);
    private:
        static ResourceType GetResourceTypeFromExtension(const std::filesystem::path& path);
    private:
        static ResourceImporter s_Importer;
    };

}