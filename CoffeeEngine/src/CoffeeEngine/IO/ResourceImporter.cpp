#include "ResourceImporter.h"
#include "CoffeeEngine/Project/Project.h"
#include <filesystem>
#include <fstream>

namespace Coffee {

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
            Ref<Resource> resource = CreateRef<Resource>();
            ResourceImporter::SaveToCache(path, resource);
            return resource;
        }
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