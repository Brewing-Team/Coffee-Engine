#include "ResourceImporter.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Project/Project.h"
#include <filesystem>
#include <fstream>

namespace Coffee {

    template<typename T>
    Ref<Resource> ResourceImporter::Import(const std::filesystem::path& path)
    {
        const std::filesystem::path& projectPath = Project::GetProjectDirectory();
        std::filesystem::path cachedPath = projectPath / (".CoffeeEngine/cache/resources/");
        std::filesystem::create_directories(cachedPath);
        std::filesystem::path cachedFilePath = cachedPath / (path.filename().string() + ".res");

        if(std::filesystem::exists(cachedFilePath))
        {
            return LoadFromCache(path);
        }
        else
        {
            return LoadFromFile<T>(path);
        }
    }

    template<>
    Ref<Resource> ResourceImporter::Import<Texture>(const std::filesystem::path& path)
    {
        const std::filesystem::path& projectPath = Project::GetProjectDirectory();
        std::filesystem::path cachedPath = projectPath / (".CoffeeEngine/cache/resources/");
        std::filesystem::create_directories(cachedPath);
        std::filesystem::path cachedFilePath = cachedPath / (path.filename().string() + ".res");

        if(std::filesystem::exists(cachedFilePath))
        {
            return LoadFromCache(path);
        }
        else
        {
            const Ref<Resource>& r = LoadFromFile<Texture>(path);
            SaveToCache(path, r);
            return r;
        }
    }

    template<typename T>
    Ref<Resource> ResourceImporter::LoadFromFile(const std::filesystem::path& path)
    {
       return CreateRef<T>(path);
    }

    const Ref<Resource>& ResourceImporter::LoadFromCache(const std::filesystem::path &path)
    {
        const std::filesystem::path& projectPath = Project::GetProjectDirectory();
        std::filesystem::path cachedPath = projectPath / (".CoffeeEngine/cache/resources/");
        std::filesystem::create_directories(cachedPath);
        std::filesystem::path cachedFilePath = cachedPath / (path.filename().string() + ".res");

        std::ifstream file(cachedFilePath, std::ios::binary);
        cereal::BinaryInputArchive archive(file);
        Ref<Resource> resource = CreateRef<Resource>();
        archive(resource);
        return resource;
    }

    void ResourceImporter::SaveToCache(const std::filesystem::path& path, Ref<Resource> resource)
    {
        const std::filesystem::path& projectPath = Project::GetProjectDirectory();
        std::filesystem::path cachedPath = projectPath / (".CoffeeEngine/cache/resources/");
        std::filesystem::create_directories(cachedPath);
        std::filesystem::path cachedFilePath = cachedPath / (path.filename().string() + ".res");

        std::ofstream file{cachedFilePath, std::ios::binary};
        cereal::BinaryOutputArchive oArchive(file);

        oArchive(resource);
    }

}