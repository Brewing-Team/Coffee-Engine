/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Resources/ImportData/ImportData.h"
#include "CoffeeEngine/Resources/Resource.h"
#include "CoffeeEngine/Resources/ResourceFormat.h"
#include "CoffeeEngine/Resources/ResourceSaver.h"
#include "CoffeeEngine/Resources/CacheManager.h"
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <filesystem>
#include <fstream>

namespace Coffee {

    class Model;
    class Mesh;
    struct Vertex;

    class Material;
    struct PBRMaterialTextures;
    class Texture;
    class Texture2D;

    /**
     * @class ResourceImporter
     * @brief Handles the import of resources such as textures.
     */
    class ResourceLoader
    {
    public:

        // TODO: Consider changing the name of functions to Load instead of Import.

        template<typename T>
        ResourceRef<T> Import(const ImportData& importData)
        {
            ImportData& data = const_cast<ImportData&>(importData);

            if (data.IsValid())
            {
                // Check if the resource is cached
                if(std::filesystem::exists(data.cachedPath))
                {
                    const ResourceRef<T>& resource = LoadFromCache<T>(data.cachedPath);
                    return resource;
                }
                // If the resource is not cached, import it
                else
                {
                    // This is a placeholder implementation this should be done for each type of resource and should be done in a separate function

                    // TODO: Think about passing the import data to the resource constructor!!! Can simplify a lot of things

                    ResourceRef<T> resource = CreateRef<T>(data);
                    
                    if(data.cache)
                        ResourceSaver::Save<T>(data.cachedPath, resource);

                    return resource;
                }
            }
            else
            {
                ResourceRef<T> resource = CreateRef<T>(data);

                data.uuid = resource->GetUUID();

                if(data.cache)
                {
                    std::filesystem::path cachedFilePath = CacheManager::GetCachedFilePath(data.uuid, GetResourceType<T>());
                    data.cachedPath = cachedFilePath;
                    
                    ResourceSaver::Save<T>(cachedFilePath, resource);
                }

                return resource;
            }
        }

        template<typename T>
        ResourceRef<T> ImportEmbedded(const ImportData& importData)
        {
            ImportData& data = const_cast<ImportData&>(importData);

            if (data.IsValid())
            {
                // Check if the resource is cached
                if (std::filesystem::exists(data.cachedPath))
                {
                    const ResourceRef<T>& resource = LoadFromCache<T>(data.cachedPath);
                    return resource;
                }
                // If the resource is not cached, import it
                else
                {
                    // Create the resource from the embedded data
                    ResourceRef<T> resource = CreateRef<T>(data);

                    // Save the resource to the cache
                    ResourceSaver::Save<T>(data.cachedPath, resource);
                    return resource;
                }
            }
            else
            {
                // Handle invalid data case
                COFFEE_WARN("ImportEmbeddedResource: Invalid import data.");
                return nullptr;
            }
        }

        template<typename T>
        ResourceRef<T> ImportFromCache(ResourceID uuid)
        {
            std::filesystem::path cachedFilePath = CacheManager::GetCachedFilePath(uuid, GetResourceType<T>());

            if (std::filesystem::exists(cachedFilePath))
            {
                const ResourceRef<Resource>& resource = LoadFromCache<T>(cachedFilePath);
                return std::static_pointer_cast<T>(resource);
            }
            else
            {
                COFFEE_ERROR("ResourceImporter::ImportResourceFromCache: Resource {0} not found in cache.", (uint64_t)uuid);
                return nullptr;
            }
        }

    private:
        /**
         * @brief Loads a resource from the cache.
         * @param path The file path of the resource to load.
         * @param format The format of the resource.
         * @return A reference to the loaded resource.
         */
        template<typename T>
        ResourceRef<T> LoadFromCache(const std::filesystem::path& path)
        {
            COFFEE_INFO("Loading resource from cache: {0}", path.string());

            ResourceFormat resourceFormat = GetResourceSaveFormatFromType(GetResourceType<T>());

            switch (resourceFormat)
            {
                case ResourceFormat::Binary:
                    return BinaryDeserialization<T>(path);
                    break;
                case ResourceFormat::JSON:
                    return JSONDeserialization<T>(path);
                    break;
            }
        }

        /**
         * @brief Deserializes a resource from a binary file.
         * @param path The file path of the binary file.
         * @return A reference to the deserialized resource.
         */
        template<typename T>
        ResourceRef<T> BinaryDeserialization(const std::filesystem::path& path)
        {
            std::ifstream file(path, std::ios::binary);
            cereal::BinaryInputArchive archive(file);
            ResourceRef<T> resource;
            archive(resource);
            return resource;
        }

        /**
         * @brief Deserializes a resource from a JSON file.
         * @param path The file path of the JSON file.
         * @return A reference to the deserialized resource.
         */
        template<typename T>
        ResourceRef<T> JSONDeserialization(const std::filesystem::path& path)
        {
            std::ifstream file(path);
            cereal::JSONInputArchive archive(file);
            ResourceRef<T> resource;
            archive(resource);
            return resource;
        }
    };
}

/** @} */