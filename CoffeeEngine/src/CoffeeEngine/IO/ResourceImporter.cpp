#include "ResourceImporter.h"
#include "ResourceSaver.h"
#include "CoffeeEngine/IO/CacheManager.h"

#include <fstream>

namespace Coffee {

    Ref<Texture> ResourceImporter::ImportTexture(const std::filesystem::path& path, bool srgb, bool cache)
    {
        if (!cache)
        {
            return CreateRef<Texture>(path, srgb);
        }

        std::filesystem::path cachedFilePath = CacheManager::GetCachedFilePath(path.filename().string());

        if (std::filesystem::exists(cachedFilePath))
        {
            const Ref<Resource>& resource = LoadFromCache(cachedFilePath, ResourceFormat::Binary);
            return std::static_pointer_cast<Texture>(resource);
        }
        else
        {
            Ref<Texture> texture = CreateRef<Texture>(path, srgb);
            ResourceSaver::SaveToCache(texture); //TODO: Add the UUID to the cache filename
            return texture;
        }
    }

    Ref<Resource> ResourceImporter::LoadFromCache(const std::filesystem::path& path, ResourceFormat format)
        {
            switch (format)
            {
                case ResourceFormat::Binary:
                    return BinaryDeserialization(path);
                    break;
                case ResourceFormat::JSON:
                    return JSONDeserialization(path);
                    break;
            }
        }

    Ref<Resource> ResourceImporter::BinaryDeserialization(const std::filesystem::path& path)
    {
        std::ifstream file(path, std::ios::binary);
        cereal::BinaryInputArchive archive(file);
        Ref<Resource> resource;
        archive(resource);
        return resource;
    }

    Ref<Resource> ResourceImporter::JSONDeserialization(const std::filesystem::path& path)
    {
        std::ifstream file(path);
        cereal::JSONInputArchive archive(file);
        Ref<Resource> resource;
        archive(resource);
        return resource;
    }
}