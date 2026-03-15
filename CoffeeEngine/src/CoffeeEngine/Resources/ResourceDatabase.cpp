#include "ResourceDatabase.h"

#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Resources/ResourceUtils.h"

#include <cereal/archives/json.hpp>
#include <algorithm>
#include <fstream>

namespace Coffee {

    namespace {
        struct DatabaseStorage
        {
            std::vector<ResourceMetadata> entries;

            template<typename Archive>
            void serialize(Archive& archive, std::uint32_t const)
            {
                archive(entries);
            }
        };
    }

    ResourceDatabase::ResourceDatabase()
        : m_DatabasePath(std::filesystem::current_path() / ".CoffeeEngine" / "resource_database.json")
    {
    }

    void ResourceDatabase::SetDatabasePath(const std::filesystem::path& databasePath)
    {
        m_DatabasePath = databasePath;
    }

    bool ResourceDatabase::Load()
    {
        m_Metadata.clear();
        m_SourceIndex.clear();
        m_ReverseDependencies.clear();

        if (!std::filesystem::exists(m_DatabasePath))
            return true;

        std::ifstream file(m_DatabasePath);
        if (!file.is_open())
        {
            COFFEE_CORE_ERROR("ResourceDatabase::Load: Could not open database file {0}", m_DatabasePath.string());
            return false;
        }

        DatabaseStorage storage;
        cereal::JSONInputArchive archive(file);
        archive(storage);

        for (const auto& metadata : storage.entries)
        {
            m_Metadata[metadata.id] = metadata;
            m_SourceIndex[MakeSourceKey(metadata.sourcePath, metadata.type)] = metadata.id;

            for (const ResourceID dependency : metadata.dependencies)
            {
                m_ReverseDependencies[dependency].push_back(metadata.id);
            }
        }

        return true;
    }

    bool ResourceDatabase::Save() const
    {
        std::filesystem::create_directories(m_DatabasePath.parent_path());

        std::ofstream file(m_DatabasePath);
        if (!file.is_open())
        {
            COFFEE_CORE_ERROR("ResourceDatabase::Save: Could not open database file {0}", m_DatabasePath.string());
            return false;
        }

        DatabaseStorage storage;
        storage.entries.reserve(m_Metadata.size());
        for (const auto& [_, metadata] : m_Metadata)
        {
            storage.entries.push_back(metadata);
        }

        cereal::JSONOutputArchive archive(file);
        archive(storage);

        return true;
    }

    std::optional<ResourceID> ResourceDatabase::FindBySourcePath(const std::filesystem::path& sourcePath, ResourceType type) const
    {
        const std::string key = MakeSourceKey(sourcePath, type);
        auto it = m_SourceIndex.find(key);
        if (it == m_SourceIndex.end())
            return std::nullopt;

        return it->second;
    }

    const ResourceMetadata* ResourceDatabase::Get(ResourceID id) const
    {
        auto it = m_Metadata.find(id);
        if (it == m_Metadata.end())
            return nullptr;

        return &it->second;
    }

    ResourceMetadata* ResourceDatabase::GetMutable(ResourceID id)
    {
        auto it = m_Metadata.find(id);
        if (it == m_Metadata.end())
            return nullptr;

        return &it->second;
    }

    ResourceMetadata& ResourceDatabase::GetOrCreate(ResourceID id, ResourceType type)
    {
        auto [it, inserted] = m_Metadata.emplace(id, ResourceMetadata{});
        if (inserted)
        {
            it->second.id = id;
            it->second.type = type;
        }

        return it->second;
    }

    void ResourceDatabase::Upsert(const ResourceMetadata& metadata)
    {
        m_Metadata[metadata.id] = metadata;
        m_SourceIndex[MakeSourceKey(metadata.sourcePath, metadata.type)] = metadata.id;

        for (const ResourceID dependency : metadata.dependencies)
        {
            auto& dependents = m_ReverseDependencies[dependency];
            if (std::find(dependents.begin(), dependents.end(), metadata.id) == dependents.end())
            {
                dependents.push_back(metadata.id);
            }
        }
    }

    void ResourceDatabase::RegisterDependency(ResourceID owner, ResourceID dependency)
    {
        ResourceMetadata* metadata = GetMutable(owner);
        if (!metadata)
            return;

        if (std::find(metadata->dependencies.begin(), metadata->dependencies.end(), dependency) == metadata->dependencies.end())
        {
            metadata->dependencies.push_back(dependency);
        }

        auto& dependents = m_ReverseDependencies[dependency];
        if (std::find(dependents.begin(), dependents.end(), owner) == dependents.end())
        {
            dependents.push_back(owner);
        }
    }

    std::vector<ResourceID> ResourceDatabase::GetDependencies(ResourceID owner) const
    {
        const ResourceMetadata* metadata = Get(owner);
        if (!metadata)
            return {};

        return metadata->dependencies;
    }

    std::vector<ResourceID> ResourceDatabase::GetDependents(ResourceID dependency) const
    {
        auto it = m_ReverseDependencies.find(dependency);
        if (it == m_ReverseDependencies.end())
            return {};

        return it->second;
    }

    std::string ResourceDatabase::MakeSourceKey(const std::filesystem::path& sourcePath, ResourceType type) const
    {
        std::filesystem::path normalized = sourcePath.lexically_normal();
        return ResourceTypeToString(type) + ":" + normalized.generic_string();
    }

}