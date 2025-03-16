/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/ResourceUtils.h"
#include "CoffeeEngine/IO/CacheManager.h"
#include <cereal/archives/json.hpp>
#include <fstream>

namespace Coffee
{

    /**
     * @class ResourceSaver
     * @brief Handles saving resources to disk and cache.
     */
	class ResourceSaver
	{
      public:
        /**
         * @brief Saves a resource to a specified path on disk.
         * @param path The file path where the resource will be saved.
         * @param resource A reference to the resource to save.
         */
        template<typename T>
        static void Save(const std::filesystem::path& path, const Ref<T>& resource)
        {
            std::filesystem::create_directories(path.parent_path());
            ResourceFormat format = GetResourceSaveFormatFromType(GetResourceType<T>());
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

        /**
         * @brief Saves a resource to the project cache.
         * @param resource A reference to the resource to save to cache.
         */
        template<typename T>
        static void SaveToCache(const std::string& filename, const Ref<T>& resource)
        {
            std::filesystem::path cacheFilePath = CacheManager::GetCachedFilePath(filename);

            Save<T>(cacheFilePath, resource);
        }

        /**
         * @brief Saves a resource to the project cache.
         * @param resource A reference to the resource to save to cache.
         */
         template<typename T>
         static void SaveToCache(UUID uuid, const Ref<T>& resource)
         {
             std::filesystem::path cacheFilePath = CacheManager::GetCachedFilePath(uuid, GetResourceType<T>());
 
             Save<T>(cacheFilePath, resource);
         }
      private:
        /**
         * @brief Serializes a resource to a binary file.
         * @param path The file path where the resource will be saved.
         * @param resource A reference to the resource to serialize.
         */
        template<typename T>
        static void BinarySerialization(const std::filesystem::path& path, const Ref<T>& resource)
        {
            std::ofstream file{path, std::ios::binary};
            cereal::BinaryOutputArchive oArchive(file);
            oArchive(resource);
        }

        /**
         * @brief Serializes a resource to a JSON file.
         * @param path The file path where the resource will be saved.
         * @param resource A reference to the resource to serialize.
         */
        template<typename T>
        static void JSONSerialization(const std::filesystem::path& path, const Ref<T>& resource)
        {
            std::ofstream file{path};
            cereal::JSONOutputArchive oArchive(file);
            oArchive(resource);
        }
	};

}

/** @} */