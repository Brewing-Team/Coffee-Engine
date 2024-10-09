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
            const Ref<Resource>& r = ResourceImporter::Import<T>(path, std::forward<Args>(args)...);

            if (r->GetType() != GetResourceType(path))
            {
                COFFEE_CORE_ERROR("ResourceLoader::Load: Resource is not of the specified type!");
                return nullptr;
            }

            return std::static_pointer_cast<T>(r);
        }
    private:
        static ResourceType GetResourceType(const std::filesystem::path& path);
    };

}