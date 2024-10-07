#pragma once 

#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include <filesystem>
namespace Coffee {

    class ResourceLoader
    {
    public:
        static void LoadResources(const std::filesystem::path& directory);
        static Ref<Resource> LoadResource(const std::filesystem::path& path);

        template <typename T>
        static Ref<T> Load(const std::filesystem::path& path);
    private:
        static ResourceType GetResourceType(const std::filesystem::path& path);
    };

}