/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"

namespace Coffee
{

    /*
        [x] Create a Save function for saving a resource on disk
        [x] Create a SaveToCache function that saves resource to the project cache
        [ ] Create a way to detect the Resource type and save it in the correct type (Binary or Json)
        [ ] Create private functions to manage the different types of serialization
        [ ] Use this class in the other Resource Management classes
    */

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
        static void Save(const std::filesystem::path& path, const Ref<Resource>& resource);

        /**
         * @brief Saves a resource to the project cache.
         * @param resource A reference to the resource to save to cache.
         */
        static void SaveToCache(const std::string& filename, const Ref<Resource>& resource);
	    static void SaveToCache(const UUID& uuid, const Ref<Resource>& resource);
      private:
        /**
         * @brief Serializes a resource to a binary file.
         * @param path The file path where the resource will be saved.
         * @param resource A reference to the resource to serialize.
         */
        static void BinarySerialization(const std::filesystem::path& path, const Ref<Resource>& resource);

        /**
         * @brief Serializes a resource to a JSON file.
         * @param path The file path where the resource will be saved.
         * @param resource A reference to the resource to serialize.
         */
        static void JSONSerialization(const std::filesystem::path& path, const Ref<Resource>& resource);
	};

}

/** @} */