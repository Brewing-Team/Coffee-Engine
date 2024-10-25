#include "ResourceSaver.h"

namespace Coffee
{
    std::filesystem::path ResourceSaver::s_cachePath = ".CoffeeEngine/Cache/Resources";

    ResourceSaveFormat GetResourceSaveFormatFromType(ResourceType type)
    {
        switch (type)
        {
        case Coffee::ResourceType::Unknown:
            break;
        case Coffee::ResourceType::Texture:
            return ResourceSaveFormat::Binary;
            break;
        case Coffee::ResourceType::Model:
            break;
        case Coffee::ResourceType::Shader:
            break;
        default:
            break;
        }
    }

    void ResourceSaver::Save(const std::filesystem::path& path, const Ref<Resource>& resource)
    {
        ResourceSaveFormat format = GetResourceSaveFormatFromType(resource->GetType());
        switch (format)
        {
            using enum ResourceSaveFormat;
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
    void ResourceSaver::SaveToCache(const Ref<Resource>& resource)
    {
        std::filesystem::create_directories(s_cachePath);
        //TODO: Research how to do it without copying the name string to resourceName (how to do it with const ... &)
        std::string resourceName = resource->GetName();
        std::filesystem::path cacheFilePath = s_cachePath / (resourceName += ".res");

        Save(cacheFilePath, resource);
    }
}