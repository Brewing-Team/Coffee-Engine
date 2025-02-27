/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/ImportData/ImportData.h"
#include "CoffeeEngine/IO/ImportData/Texture2DImportData.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/IO/ResourceFormat.h"
#include "CoffeeEngine/IO/ResourceSaver.h"
#include "CoffeeEngine/Math/BoundingBox.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/IO/CacheManager.h"
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <filesystem>
#include <string>

namespace Coffee {

    class Model;
    class Mesh;
    struct Vertex;

    class Material;
    struct MaterialTextures;
    class Texture;
    class Texture2D;

    /**
     * @class ResourceImporter
     * @brief Handles the import of resources such as textures.
     */
    class ResourceImporter
    {
    public:


        template<typename T>
        Ref<T> Import(const ImportData& importData)
        {
            ImportData& data = const_cast<ImportData&>(importData);

            if (data.IsValid())
            {
                // Check if the resource is cached
                if(std::filesystem::exists(data.cachedPath))
                {
                    const Ref<T>& resource = LoadFromCache(data.cachedPath, data.type);
                    return resource;
                }
                // If the resource is not cached, import it
                else
                {
                    // This is a placeholder implementation this should be done for each type of resource and should be done in a separate function

                    // TODO: Think about passing the import data to the resource constructor!!! Can simplify a lot of things

                    Ref<T> resource = CreateRef<T>(data);

                    ResourceSaver::SaveToCache(data.cachedPath, resource);
                }
            }
            else
            {
                Ref<Texture2D> texture = CreateRef<Texture2D>(data);

                data.uuid = texture->GetUUID();

                std::filesystem::path cachedFilePath = CacheManager::GetCachedFilePath(texture->GetName(), data.uuid, ResourceType::Texture2D);

                data.cachedPath = cachedFilePath;

                ResourceSaver::SaveToCache(texture->GetName(), data.uuid, texture);
            }
        }

        template<typename T>
        Ref<T> ImportEmbedded(const ImportData& importData)
        {
            ImportData& data = const_cast<ImportData&>(importData);

            if (data.IsValid())
            {
                // Check if the resource is cached
                if (std::filesystem::exists(data.cachedPath))
                {
                    const Ref<T>& resource = LoadFromCache(data.cachedPath, data.type);
                    return resource;
                }
                // If the resource is not cached, import it
                else
                {
                    // Create the resource from the embedded data
                    Ref<T> resource = CreateRef<T>(data);

                    // Save the resource to the cache
                    ResourceSaver::SaveToCache(data.cachedPath, resource);
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

        /**
         * @brief Imports a texture from a given file path.
         * @param path The file path of the texture to import.
         * @param srgb Whether the texture should be imported in sRGB format.
         * @param cache Whether the texture should be cached.
         * @return A reference to the imported texture.
         */
        Ref<Texture2D> ImportTexture2D(const std::filesystem::path& path, const UUID& uuid, bool srgb, bool cache);
        Ref<Texture2D> ImportTexture2D(const UUID& uuid);
        Ref<Cubemap> ImportCubemap(const std::filesystem::path& path, const UUID& uuid);
        Ref<Cubemap> ImportCubemap(const UUID& uuid);
        Ref<Model> ImportModel(const std::filesystem::path& path, bool cache);
        Ref<Mesh> ImportMesh(const std::string& name, const UUID& uuid, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Ref<Material>& material, const AABB& aabb);
        Ref<Mesh> ImportMesh(const UUID& uuid);

        Ref<Material> ImportMaterial(const std::string& name, const UUID& uuid);
        Ref<Material> ImportMaterial(const std::string& name, const UUID& uuid, MaterialTextures& materialTextures);
        Ref<Material> ImportMaterial(const UUID& uuid);
    private:
        /**
         * @brief Loads a resource from the cache.
         * @param path The file path of the resource to load.
         * @param format The format of the resource.
         * @return A reference to the loaded resource.
         */
        Ref<Resource> LoadFromCache(const std::filesystem::path& path, ResourceType type);

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