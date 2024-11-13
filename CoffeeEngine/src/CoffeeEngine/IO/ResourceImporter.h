/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/IO/ResourceFormat.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

namespace Coffee {

    /**
     * @class ResourceImporter
     * @brief Handles the import of resources such as textures.
     */
    class ResourceImporter
    {
    public:
        /**
         * @brief Imports a texture from a given file path.
         * @param path The file path of the texture to import.
         * @param srgb Whether the texture should be imported in sRGB format.
         * @param cache Whether the texture should be cached.
         * @return A reference to the imported texture.
         */
        Ref<Texture> ImportTexture(const std::filesystem::path& path, bool srgb, bool cache);

        Ref<Mesh> ImportMesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    private:
        /**
         * @brief Loads a resource from the cache.
         * @param path The file path of the resource to load.
         * @param format The format of the resource.
         * @return A reference to the loaded resource.
         */
        Ref<Resource> LoadFromCache(const std::filesystem::path& path, ResourceFormat format);

        /**
         * @brief Deserializes a resource from a binary file.
         * @param path The file path of the binary file.
         * @return A reference to the deserialized resource.
         */
        Ref<Resource> BinaryDeserialization(const std::filesystem::path& path);

        /**
         * @brief Deserializes a resource from a JSON file.
         * @param path The file path of the JSON file.
         * @return A reference to the deserialized resource.
         */
        Ref<Resource> JSONDeserialization(const std::filesystem::path& path);
    };
}

/** @} */