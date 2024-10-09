#pragma once 

#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/IO/ResourceImporter.h"
#include <filesystem>
namespace Coffee {

    class ResourceLoader
    {
    public:
        static void LoadResources(const std::filesystem::path& directory);
        static Ref<Resource> LoadResource(const std::filesystem::path& path);

        template <typename T, typename... Args>
        static Ref<T> Load(const std::filesystem::path& path, Args&&... args)
        {
            const Ref<T>& resource = ResourceImporter::Import<T>(path, std::forward<Args>(args)...);

            if (resource->GetType() != GetResourceTypeFromExtension(path))
            {
                COFFEE_CORE_ERROR("ResourceLoader::Load: Resource is not of the specified type!");
                return nullptr;
            }

            return resource;
        }
    private:
        static ResourceType GetResourceTypeFromExtension(const std::filesystem::path& path);
    };

}