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
            const std::filesystem::path& projectPath = Project::GetProjectDirectory();
            std::filesystem::path cachedPath = projectPath / (".CoffeeEngine/cache/resources/");
            std::filesystem::create_directories(cachedPath);
            std::filesystem::path cachedFilePath = cachedPath / (path.filename().string() + ".res");

            if (std::filesystem::exists(cachedFilePath))
            {
                return std::static_pointer_cast<T>(LoadFromCache(path));
            }
            else
            {
                Ref<T> resource = LoadFromFile<T>(path, std::forward<Args>(args)...);
                SaveToCache(path, resource);
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