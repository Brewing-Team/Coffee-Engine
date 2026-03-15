#pragma once

#include "CoffeeEngine/Resources/Interfaces/IResourceDatabase.h"
#include "CoffeeEngine/Resources/Resource.h"

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Coffee {

    struct ResourceMetadata
    {
        ResourceID id = ResourceID::null;
        ResourceType type = ResourceType::Unknown;

        std::filesystem::path sourcePath;
        std::filesystem::path cachedPath;

        std::uint64_t importSettingsHash = 0;
        std::uint64_t contentHash = 0;
        std::uint32_t importerVersion = 1;

        bool internal = false;
        std::vector<ResourceID> dependencies;

        template<typename Archive>
        void serialize(Archive& archive, std::uint32_t const)
        {
            archive(id,
                    type,
                    sourcePath,
                    cachedPath,
                    importSettingsHash,
                    contentHash,
                    importerVersion,
                    internal,
                    dependencies);
        }
    };

    class ResourceDatabase : public IResourceDatabase
    {
    public:
        ResourceDatabase();

        void SetDatabasePath(const std::filesystem::path& databasePath);
        const std::filesystem::path& GetDatabasePath() const { return m_DatabasePath; }

        bool Load();
        bool Save() const;

        std::optional<ResourceID> FindBySourcePath(const std::filesystem::path& sourcePath, ResourceType type) const;

        const ResourceMetadata* Get(ResourceID id) const;
        ResourceMetadata* GetMutable(ResourceID id);

        ResourceMetadata& GetOrCreate(ResourceID id, ResourceType type);
        void Upsert(const ResourceMetadata& metadata);

        void RegisterDependency(ResourceID owner, ResourceID dependency);
        std::vector<ResourceID> GetDependencies(ResourceID owner) const;
        std::vector<ResourceID> GetDependents(ResourceID dependency) const;

    private:
        std::string MakeSourceKey(const std::filesystem::path& sourcePath, ResourceType type) const;

    private:
        std::filesystem::path m_DatabasePath;
        std::unordered_map<ResourceID, ResourceMetadata> m_Metadata;
        std::unordered_map<std::string, ResourceID> m_SourceIndex;
        std::unordered_map<ResourceID, std::vector<ResourceID>> m_ReverseDependencies;
    };

}