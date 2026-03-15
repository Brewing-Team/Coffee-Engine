/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include "ResourceDatabase.h"
#include "ResourceDependencyGraph.h"
#include "ResourceImportService.h"
#include "Interfaces/IResourceManager.h"
#include "ResourceRegistry.h"
#include "ResourceLoader.h"
#include "ResourceResolver.h"

#include "CoffeeEngine/Core/Base.h"
#include "ImportData/ImportDataUtils.h"

#include <filesystem>
#include <future>
#include <optional>
#include <unordered_map>

namespace Coffee {
    class ImportData;
    class Scene;
}

namespace Coffee {

    /**
     * @class ResourceLoader
     * @brief Loads resources such as textures and models for the CoffeeEngine.
     */
    class ResourceManager : public IResourceManager
    {
    public:

        ResourceManager();    

        ResourceHandle LoadResource(const std::filesystem::path& path, ResourceType type);

        template<typename T>
        ResourceHandle LoadResource(const std::filesystem::path& path)
        {
            const ResourceRef<T>& resource = Load<T>(path);
            if (!resource)
                return {};

            return {resource->GetUUID(), ++m_Generation};
        }

        std::future<ResourceHandle> LoadResourceAsync(const std::filesystem::path& path, ResourceType type);

        template<typename T>
        std::future<ResourceHandle> LoadResourceAsync(const std::filesystem::path& path)
        {
            return std::async(std::launch::async, [this, path]() {
                return this->LoadResource(path, GetResourceType<T>());
            });
        }

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
        inline ResourceRef<T> Load(const std::filesystem::path& path)
        {
            std::filesystem::path absolutePath = path;
            if (!absolutePath.is_absolute())
                absolutePath = m_WorkingDirectory / absolutePath;

            absolutePath = absolutePath.lexically_normal();

            if (const std::optional<ResourceID> existing = m_Database.FindBySourcePath(absolutePath, GetResourceType<T>());
                existing.has_value())
            {
                if (m_Registry.Exists(*existing))
                {
                    return m_Registry.Get<T>(*existing);
                }
            }

            if (ImportDataUtils::HasImportFile(absolutePath))
            {
                std::filesystem::path importPath = absolutePath;
                importPath += ".import";
                Scope<ImportData> importData = ImportDataUtils::LoadImportData(importPath);
                ResourceRef<T> resource = Load<T>(*importData);

                if (resource)
                {
                    ResourceMetadata metadata;
                    metadata.id = resource->GetUUID();
                    metadata.type = GetResourceType<T>();
                    metadata.sourcePath = importData->originalPath;
                    metadata.cachedPath = importData->cachedPath;
                    metadata.internal = importData->internal;
                    m_Database.Upsert(metadata);
                    m_LoadStates[resource->GetUUID()] = ResourceLoadState::Loaded;
                }

                return resource;
            }
            else
            {
                Scope<ImportData> newImportData = ImportDataUtils::CreateImportData<T>();
                newImportData->originalPath = absolutePath;

                if(isInternalResource(absolutePath))
                {
                    newImportData->internal = true;
                }

                const ResourceRef<T>& resource = m_ImportService.Import<T>(*newImportData);

                if (!resource)
                {
                    m_LoadStates[newImportData->uuid] = ResourceLoadState::Failed;
                    return nullptr;
                }

                ImportDataUtils::SaveImportData(newImportData);
                m_Registry.Add(newImportData->uuid, resource);
                ResolveIfNeeded(resource);

                ResourceMetadata metadata;
                metadata.id = newImportData->uuid;
                metadata.type = GetResourceType<T>();
                metadata.sourcePath = newImportData->originalPath;
                metadata.cachedPath = newImportData->cachedPath;
                metadata.internal = newImportData->internal;
                m_Database.Upsert(metadata);
                m_LoadStates[newImportData->uuid] = ResourceLoadState::Loaded;

                return resource;
            }
        }

        template<typename T>
        inline ResourceRef<T> Load(const ImportData& importData)
        {
            if (m_Registry.Exists(importData.uuid))
            {
                return m_Registry.Get<T>(importData.uuid);
            }

            const ResourceRef<T>& resource = m_ImportService.Import<T>(importData);

            if (!resource)
            {
                m_LoadStates[importData.uuid] = ResourceLoadState::Failed;
                return nullptr;
            }
            
            m_Registry.Add(importData.uuid, resource);
            ResolveIfNeeded(resource);

            ResourceMetadata metadata;
            metadata.id = importData.uuid;
            metadata.type = importData.type;
            metadata.sourcePath = importData.originalPath;
            metadata.cachedPath = importData.cachedPath;
            metadata.internal = importData.internal;
            m_Database.Upsert(metadata);
            m_LoadStates[importData.uuid] = ResourceLoadState::Loaded;

            return resource;
        }

        template<typename T>
        ResourceRef<T> LoadEmbedded(const ImportData& importData)
        {
            if (m_Registry.Exists(importData.uuid))
            {
                return m_Registry.Get<T>(importData.uuid);
            }

            const ResourceRef<T>& resource = m_ImportService.ImportEmbedded<T>(importData);

            if (!resource)
            {
                m_LoadStates[importData.uuid] = ResourceLoadState::Failed;
                return nullptr;
            }

            m_Registry.Add(importData.uuid, resource);
            ResolveIfNeeded(resource);

            ResourceMetadata metadata;
            metadata.id = importData.uuid;
            metadata.type = importData.type;
            metadata.sourcePath = importData.originalPath;
            metadata.cachedPath = importData.cachedPath;
            metadata.internal = importData.internal;
            m_Database.Upsert(metadata);
            m_LoadStates[importData.uuid] = ResourceLoadState::Loaded;

            return resource;
        }

        template<typename T>
        ResourceRef<T> GetResource(ResourceID uuid)
        {
            if (uuid == ResourceID::null)
                return nullptr;

            if (m_Registry.Exists(uuid))
            {
                return m_Registry.Get<T>(uuid);
            }

            const ResourceRef<T>& resource = m_ImportService.ImportFromCache<T>(uuid);

            if (resource)
            {
                m_Registry.Add(uuid, resource);
                ResolveIfNeeded(resource);
                m_LoadStates[uuid] = ResourceLoadState::Loaded;
                return resource;
            }

            m_LoadStates[uuid] = ResourceLoadState::Failed;

            return nullptr;
        }

        template<typename T>
        ResourceRef<T> GetResource(const std::string& name)
        {
            if (name.empty())
                return nullptr;

            if (!m_Registry.Exists(name))
                return nullptr;

            return m_Registry.Get<T>(name);
        }

        template<typename T>
        ResourceRef<T> GetOrLoad(ResourceID id, const std::optional<std::filesystem::path>& path = std::nullopt)
        {
            if (ResourceRef<T> loaded = GetResource<T>(id))
                return loaded;

            if (path.has_value())
                return Load<T>(*path);

            return nullptr;
        }

        ResourceLoadState GetLoadState(ResourceID id) const
        {
            auto it = m_LoadStates.find(id);
            if (it == m_LoadStates.end())
                return ResourceLoadState::Unloaded;

            return it->second;
        }

        void ResolveSceneResourceReferences(Scene& scene);

        void RemoveResource(const ResourceRef<Resource>& resource);
        void ReimportResource(const ResourceRef<Resource>& resource);

        void ClearRegistry() { m_Registry.Clear(); }
        const ResourceRegistry& GetRegistry() const { return m_Registry; }

        void SetWorkingDirectory(const std::filesystem::path& path);
        const std::filesystem::path& GetWorkingDirectory() { return m_WorkingDirectory; }

        bool SaveDatabase() const { return m_Database.Save(); }
        bool LoadDatabase() { return m_Database.Load(); }
    
    private:
        template<typename T>
        void ResolveIfNeeded(const ResourceRef<T>& resource)
        {
            if (!resource)
                return;

            if constexpr (requires(T& value, ResourceManager & manager) { value.ResolveResources(manager); })
            {
                resource->ResolveResources(*this);
            }
        }

        bool isInternalResource(const std::filesystem::path& path);
    private:
        std::uint32_t m_Generation = 0;

        std::filesystem::path m_EngineAssetsDirectory; ///< The directory where the engine Resources are stored.
        std::filesystem::path m_WorkingDirectory; ///< The working directory of the resource loader.

        std::unordered_map<ResourceID, ResourceLoadState> m_LoadStates;

        ResourceDatabase m_Database;
        ResourceDependencyGraph m_DependencyGraph;
        ResourceRegistry m_Registry; ///< The registry that holds all loaded resources.
        ResourceLoader m_Loader; ///< The loader used to load resources.
        ResourceImportService m_ImportService;
        ResourceResolver m_Resolver;
    };

}

/** @} */