#pragma once

#include "CoffeeEngine/Resources/Resource.h"

#include <filesystem>
#include <optional>
#include <vector>

namespace Coffee {

    struct ResourceMetadata;

    class IResourceDatabase
    {
    public:
        virtual ~IResourceDatabase() = default;

        virtual bool Load() = 0;
        virtual bool Save() const = 0;

        virtual std::optional<ResourceID> FindBySourcePath(const std::filesystem::path& sourcePath, ResourceType type) const = 0;

        virtual const ResourceMetadata* Get(ResourceID id) const = 0;
        virtual ResourceMetadata* GetMutable(ResourceID id) = 0;

        virtual void Upsert(const ResourceMetadata& metadata) = 0;

        virtual std::vector<ResourceID> GetDependencies(ResourceID owner) const = 0;
        virtual std::vector<ResourceID> GetDependents(ResourceID dependency) const = 0;
    };

}