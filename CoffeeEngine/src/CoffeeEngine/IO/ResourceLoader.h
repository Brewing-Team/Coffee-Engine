/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include "CoffeeEngine/Core/UUID.h"
#include "CoffeeEngine/IO/ResourceImporter.h"
#include "CoffeeEngine/Math/BoundingBox.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "ImportData/ImportData.h"
#include "ResourceRegistry.h"

#include <filesystem>

namespace Coffee {

    class Model;
    class Mesh;
    class Material;
    class Texture;
    class Texture2D;

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
        static Ref<Texture2D> LoadTexture2D(const std::filesystem::path& path, bool srgb = true, bool cache = true);

        template <typename T>
        static Ref<T> LoadResource(const std::filesystem::path& path)
        {
            if (HasImportFile(path))
            {
                ImportData importData = LoadImportData(path);

                if (ResourceRegistry::Exists(importData.uuid))
                {
                    return ResourceRegistry::Get<T>(importData.uuid);
                }

                const Ref<T>& resource = s_Importer.Import<T>(importData);
                // TODO: Move this to the importer passing it to the constructor
                /* resource->SetUUID(importData.uuid);
                resource->SetName(path.filename().string()); */

                ResourceRegistry::Add(importData.uuid, resource);
                return resource;
            }
            else
            {
                ImportData dummyImportData;
                dummyImportData.originalPath = path;

                const Ref<T>& resource = s_Importer.Import<T>(dummyImportData);
                const ImportData& importData = resource->GetImportData();

                SaveImportData(importData);
                ResourceRegistry::Add(importData.uuid, resource);
            }
        }

        // Rename the functions that take a UUID to GetResource<type>FromUUID
        static Ref<Texture2D> LoadTexture2D(UUID uuid);

        static Ref<Cubemap> LoadCubemap(const std::filesystem::path& path);
        static Ref<Cubemap> LoadCubemap(UUID uuid);

        /**
         * @brief Loads a model from a file.
         * @param path The file path of the model to load.
         * @param cache Whether the model should be cached.
         * @return A reference to the loaded model.
         */
        static Ref<Model> LoadModel(const std::filesystem::path& path, bool cache = true);

        static Ref<Mesh> LoadMesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Ref<Material>& material, const AABB& aabb);
        static Ref<Mesh> LoadMesh(UUID uuid);

        static Ref<Shader> LoadShader(const std::filesystem::path& shaderPath);
        static Ref<Shader> LoadShader(const std::string& shaderSource);

        static Ref<Material> LoadMaterial(const std::string& name);
        static Ref<Material> LoadMaterial(const std::string& name, MaterialTextures& materialTextures);
        static Ref<Material> LoadMaterial(UUID uuid);

        static void RemoveResource(UUID uuid);
        static void RemoveResource(const std::filesystem::path& path);

        static void SetWorkingDirectory(const std::filesystem::path& path) { s_WorkingDirectory = path; }
    private:
        
        static void SaveImportData(const ImportData& importData);
        static ImportData LoadImportData(const std::filesystem::path& path);
        static bool HasImportFile(const std::filesystem::path& path)
        {
            std::filesystem::path importFilePath = path;
            importFilePath.replace_extension(".import");

            return std::filesystem::exists(importFilePath);
        }

    private:
        static std::filesystem::path s_WorkingDirectory; ///< The working directory of the resource loader.
        static ResourceImporter s_Importer; ///< The importer used to load resources.
    };

}

/** @} */