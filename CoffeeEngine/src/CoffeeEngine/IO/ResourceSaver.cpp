#include "ResourceSaver.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/IO/ResourceFormat.h"
#include "CoffeeEngine/IO/CacheManager.h"
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>

namespace Coffee
{
    void ResourceSaver::Save(const std::filesystem::path& path, const Ref<Resource>& resource)
    {
        ResourceFormat format = GetResourceSaveFormatFromType(resource->GetType());
        switch (format)
        {
            using enum ResourceFormat;
        case Binary:
            BinarySerialization(path, resource);
            break;
        case JSON:
            JSONSerialization(path, resource);
            break;
        default:
            break;
        }
    }
    void ResourceSaver::SaveToCache(const std::string& filename, const Ref<Resource>& resource)
    {
        std::filesystem::path cacheFilePath = CacheManager::GetCachedFilePath(filename);

        Save(cacheFilePath, resource);
    }

    void ResourceSaver::SaveToCache(const UUID& uuid, const Ref<Resource>& resource)
    {
        std::filesystem::path cacheFilePath = CacheManager::GetCachedFilePath(uuid, resource->GetType());

        Save(cacheFilePath, resource);
    }

    void ResourceSaver::BinarySerialization(const std::filesystem::path& path, const Ref<Resource>& resource)
    {
        std::ofstream file{path, std::ios::binary};
        cereal::BinaryOutputArchive oArchive(file);
        oArchive(resource);
    }
    void ResourceSaver::JSONSerialization(const std::filesystem::path& path, const Ref<Resource>& resource)
    {
        std::ofstream file{path};
        cereal::JSONOutputArchive oArchive(file);
        oArchive(resource);
    }
}