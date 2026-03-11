/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include "CoffeeEngine/IO/CacheManager.h"
#include "ResourceRegistry.h"
#include "ResourceLoader.h"

#include "CoffeeEngine/Core/Base.h"
#include "ImportData/ImportDataUtils.h"

#include <filesystem>

namespace Coffee {
    class ImportData;
}

namespace Coffee {

    /**
     * @class ResourceLoader
     * @brief Loads resources such as textures and models for the CoffeeEngine.
     */
    class ResourceManager
    {
    public:

        ResourceManager();    

        /**
         * @brief Loads all resources from a directory.
         * @param directory The directory to load resources from.
         */
        void LoadDirectory(const std::filesystem::path& directory);

        /**
         * @brief Loads a single resource file.
         * @param path The file path of the resource to load.
         */
        void LoadFile(const std::filesystem::path& path);

        template <typename T>
        inline Ref<T> Load(const std::filesystem::path& path)
        {
            if (ImportDataUtils::HasImportFile(path))
            {
                std::filesystem::path importPath = path;
                importPath += ".import";
                Scope<ImportData> importData = ImportDataUtils::LoadImportData(importPath);
                return Load<T>(*importData);
            }
            else
            {
                Scope<ImportData> newImportData = ImportDataUtils::CreateImportData<T>();
                newImportData->originalPath = path;

                if(isInternalResource(path))
                {
                    newImportData->internal = true;
                }

                const Ref<T>& resource = m_Loader.Import<T>(*newImportData);

                ImportDataUtils::SaveImportData(newImportData);
                m_Registry.Add(newImportData->uuid, resource);

                return resource;
            }
        }

        template<typename T>
        inline Ref<T> Load(const ImportData& importData)
        {
            if (m_Registry.Exists(importData.uuid))
            {
                return m_Registry.Get<T>(importData.uuid);
            }

            const Ref<T>& resource = m_Loader.Import<T>(importData);
            
            m_Registry.Add(importData.uuid, resource);
            return resource;
        }

        template<typename T>
        Ref<T> LoadEmbedded(const ImportData& importData)
        {
            if (m_Registry.Exists(importData.uuid))
            {
                return m_Registry.Get<T>(importData.uuid);
            }

            const Ref<T>& resource = m_Loader.ImportEmbedded<T>(importData);

            m_Registry.Add(importData.uuid, resource);
            return resource;
        }

        template<typename T>
        Ref<T> GetResource(UUID uuid)
        {
            if (uuid == UUID::null)
                return nullptr;

            if (m_Registry.Exists(uuid))
            {
                return m_Registry.Get<T>(uuid);
            }

            const Ref<T>& resource = m_Loader.ImportFromCache<T>(uuid);

            if (resource)
            {
                m_Registry.Add(uuid, resource);
                return resource;
            }

            return nullptr;
        }

        void RemoveResource(const Ref<Resource>& resource);
        void ReimportResource(const Ref<Resource>& resource);

        void SetWorkingDirectory(const std::filesystem::path& path) { m_WorkingDirectory = path; }
        const std::filesystem::path& GetWorkingDirectory() { return m_WorkingDirectory; }
    
    private:
        bool isInternalResource(const std::filesystem::path& path);
    private:
        std::filesystem::path m_EngineAssetsDirectory; ///< The directory where the engine Resources are stored.
        std::filesystem::path m_WorkingDirectory; ///< The working directory of the resource loader.
        ResourceRegistry m_Registry; ///< The registry that holds all loaded resources.
        ResourceLoader m_Loader; ///< The loader used to load resources.
    };

}

/** @} */