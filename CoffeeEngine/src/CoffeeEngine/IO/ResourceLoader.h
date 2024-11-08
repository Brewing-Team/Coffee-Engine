/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/IO/ResourceImporter.h"
#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include <filesystem>

namespace Coffee {

    /**
     * @class ResourceLoader
     * @brief Loads resources such as textures and models for the CoffeeEngine.
     */
    class ResourceLoader
    {
    public:
        /**
         * @brief Loads all resources from a directory.
         * @param directory The directory to load resources from.
         */
        static void LoadDirectory(const std::filesystem::path& directory);

        /**
         * @brief Loads a single resource file.
         * @param path The file path of the resource to load.
         */
        static void LoadFile(const std::filesystem::path& path);

        /**
         * @brief Loads a texture from a file.
         * @param path The file path of the texture to load.
         * @param srgb Whether the texture should be loaded in sRGB format.
         * @param cache Whether the texture should be cached.
         * @return A reference to the loaded texture.
         */
        static Ref<Texture> LoadTexture(const std::filesystem::path& path, bool srgb = true, bool cache = true);

        /**
         * @brief Loads a model from a file.
         * @param path The file path of the model to load.
         * @param cache Whether the model should be cached.
         * @return A reference to the loaded model.
         */
        static Ref<Model> LoadModel(const std::filesystem::path& path, bool cache = true);
    private:
        /**
         * @brief Determines the resource type from the file extension.
         * @param path The file path to check.
         * @return The resource type.
         */
        static ResourceType GetResourceTypeFromExtension(const std::filesystem::path& path);
    private:
        static ResourceImporter s_Importer; ///< The importer used to load resources.
    };

}

/** @} */