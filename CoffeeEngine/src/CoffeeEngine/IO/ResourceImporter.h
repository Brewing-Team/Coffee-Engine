#pragma once

#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/Project/Project.h"
#include <memory>

namespace Coffee {

    class ResourceImporter
    {
    public:
        template<typename T, typename... Args>
        static Ref<T> Import(const std::filesystem::path& path, Args&&... args) //TODO: Think if the return type should be Ref<T> instead of Ref<Resource>
        {
            std::filesystem::path cachePath = ".CoffeeEngine/Cache/Resources";
            
            if(Project::GetActive())
            {
                cachePath = Project::GetActive()->GetCacheDirectory() / "Resources";
            }

            std::filesystem::create_directories(cachePath);

            std::filesystem::path cachedFilePath = cachePath / (path.filename().string() + ".res");

            if (std::filesystem::exists(cachedFilePath))
            {
                return std::static_pointer_cast<T>(LoadFromCache(cachedFilePath));
            }
            else
            {
                Ref<T> resource = LoadFromFile<T>(path, std::forward<Args>(args)...);
                SaveToCache(cachedFilePath, resource);
                return resource;
            }
        }
    private:
        template<typename T, typename... Args>
        static Ref<T> LoadFromFile(const std::filesystem::path& path, Args&&... args)
        {
            return CreateRef<T>(path, std::forward<Args>(args)...);
        }

        static void SaveToCache(const std::filesystem::path& path, Ref<Resource> resource);
        static Ref<Resource> LoadFromCache(const std::filesystem::path& path);
    };

}