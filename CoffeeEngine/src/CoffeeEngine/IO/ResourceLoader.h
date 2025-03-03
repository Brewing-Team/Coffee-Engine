/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/UUID.h"
#include "CoffeeEngine/IO/ResourceImporter.h"
#include "CoffeeEngine/IO/ImportData/CreateImportData.h"
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

        template <typename T>
        inline static Ref<T> Load(const std::filesystem::path& path)
        {
            if (HasImportFile(path))
            {
                Scope<ImportData> importData = LoadImportData(path);
                return Load<T>(*importData);
            }
            else
            {
                Scope<ImportData> newImportData = CreateImportData<T>();
                newImportData->originalPath = path;

                const Ref<T>& resource = s_Importer.Import<T>(*newImportData);

                SaveImportData(newImportData);
                ResourceRegistry::Add(newImportData->uuid, resource);

                return resource;
            }
        }

        template<typename T>
        inline static Ref<T> Load(const ImportData& importData)
        {
            if (ResourceRegistry::Exists(importData.uuid))
            {
                return ResourceRegistry::Get<T>(importData.uuid);
            }

            const Ref<T>& resource = s_Importer.Import<T>(importData);
            
            // ------------------TESTING----------------------
            Scope<ImportData> newImportData = CreateScope<ImportData>(importData);
            SaveImportData(newImportData); // Should I do this for the case that the importData doesn't exist on disk?
            // -----------------------------------------------

            ResourceRegistry::Add(importData.uuid, resource);
            return resource;
        }

        template<typename T>
        static Ref<T> LoadEmbedded(const ImportData& importData)
        {
            if (ResourceRegistry::Exists(importData.uuid))
            {
                return ResourceRegistry::Get<T>(importData.uuid);
            }

            const Ref<T>& resource = s_Importer.ImportEmbedded<T>(importData);

            ResourceRegistry::Add(importData.uuid, resource);
            return resource;
        }

        template<typename T>
        static Ref<T> GetResource(UUID uuid)
        {
            if (uuid == UUID::null)
                return nullptr;

            if (ResourceRegistry::Exists(uuid))
            {
                return ResourceRegistry::Get<T>(uuid);
            }

            const Ref<T>& resource = s_Importer.ImportFromCache<T>(uuid);

            if (resource)
            {
                ResourceRegistry::Add(uuid, resource);
                return resource;
            }

            return nullptr;
        }

        static void RemoveResource(UUID uuid);
        static void RemoveResource(const std::filesystem::path& path);

        static void SetWorkingDirectory(const std::filesystem::path& path) { s_WorkingDirectory = path; }
    private:
        
        static void SaveImportData(Scope<ImportData>& importData);
        static Scope<ImportData> LoadImportData(const std::filesystem::path& path);
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