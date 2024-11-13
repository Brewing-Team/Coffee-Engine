#include "ResourceImporter.h"
#include "ResourceSaver.h"
#include "CoffeeEngine/IO/CacheManager.h"

#include <filesystem>
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
            COFFEE_INFO("ResourceImporter::ImportTexture: Texture {0} not found in cache. Creating new texture.", path.string());
            Ref<Texture> texture = CreateRef<Texture>(path, srgb);
            ResourceSaver::SaveToCache(texture->GetName(), texture);
            return texture;
        }
    }

    Ref<Mesh> ResourceImporter::ImportMesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    {
        // TODO: Think about adding a cache parameter.

        std::filesystem::path cachedFilePath = CacheManager::GetCachedFilePath(name);

        if(std::filesystem::exists(cachedFilePath))
        {
            const Ref<Resource>& resource = LoadFromCache(cachedFilePath, ResourceFormat::Binary);
            return std::static_pointer_cast<Mesh>(resource);
        }
        else
        {
            COFFEE_ERROR("ResourceImporter::ImportMesh: Mesh {0} not found in cache. Creating new mesh.", name);
            Ref<Mesh> mesh = CreateRef<Mesh>(vertices, indices);
            ResourceSaver::SaveToCache(name, mesh);
            return mesh;
        }
    }

    Ref<Resource> ResourceImporter::LoadFromCache(const std::filesystem::path& path, ResourceFormat format)
        {
            COFFEE_INFO("Loading resource from cache: {0}", path.string());
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