#include "ResourceImporter.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Project/Project.h"
#include <filesystem>
#include <fstream>

namespace Coffee {

    Ref<Resource> ResourceImporter::LoadFromCache(const std::filesystem::path &path)
    {
        std::ifstream file(path, std::ios::binary);
        cereal::BinaryInputArchive archive(file);
        Ref<Resource> resource = CreateRef<Resource>();
        archive(resource);
        return resource;
    }

    void ResourceImporter::SaveToCache(const std::filesystem::path& path, Ref<Resource> resource)
    {
        std::ofstream file{path, std::ios::binary};
        cereal::BinaryOutputArchive oArchive(file);
        oArchive(resource);
    }

}