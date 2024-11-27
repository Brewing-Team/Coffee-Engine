/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include "CoffeeEngine/Core/UUID.h"
#include "CoffeeEngine/IO/ResourceImporter.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include <filesystem>

namespace Coffee {

    class Model;
    class Mesh;
    class Material;
    class Texture;

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
        static Ref<Texture> LoadTexture(UUID uuid);

        /**
         * @brief Loads a model from a file.
         * @param path The file path of the model to load.
         * @param cache Whether the model should be cached.
         * @return A reference to the loaded model.
         */
        static Ref<Model> LoadModel(const std::filesystem::path& path, bool cache = true);

        static Ref<Mesh> LoadMesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Ref<Material>& material);
        static Ref<Mesh> LoadMesh(UUID uuid);

        static Ref<Shader> LoadShader(const std::filesystem::path& shaderPath);
        static Ref<Shader> LoadShader(const std::string& shaderSource);

        static Ref<Material> LoadMaterial(const std::string& name);
        static Ref<Material> LoadMaterial(const std::string& name, MaterialTextures& materialTextures);
        static Ref<Material> LoadMaterial(UUID uuid);

        static void SetWorkingDirectory(const std::filesystem::path& path) { s_WorkingDirectory = path; }
    private:
        struct ImportData
        {
            UUID uuid;
            std::filesystem::path originalPath;

            template<typename Archive>
            void serialize(Archive& archive)
            {
                archive(CEREAL_NVP(uuid), CEREAL_NVP(originalPath));
            }
        };
        
        static void GenerateImportFile(const std::filesystem::path& path);
        static ImportData GetImportData(const std::filesystem::path& path);

        static UUID GetUUIDFromImportFile(const std::filesystem::path& path);
        static std::filesystem::path GetPathFromImportFile(const std::filesystem::path& path);
    private:
        static std::filesystem::path s_WorkingDirectory; ///< The working directory of the resource loader.
        static ResourceImporter s_Importer; ///< The importer used to load resources.
    };

}

/** @} */