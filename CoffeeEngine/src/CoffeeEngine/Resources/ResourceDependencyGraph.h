#pragma once

#include "CoffeeEngine/Resources/Resource.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Coffee {

    class ResourceDependencyGraph
    {
    public:
        void Clear();
        void RemoveResource(ResourceID id);

        void AddDependency(ResourceID owner, ResourceID dependency);

        std::vector<ResourceID> GetDependencies(ResourceID owner) const;
        std::vector<ResourceID> GetDependents(ResourceID dependency) const;

        void MarkDirty(ResourceID id);
        bool IsDirty(ResourceID id) const;
        void ClearDirty(ResourceID id);

    private:
        std::unordered_map<ResourceID, std::unordered_set<ResourceID>> m_Dependencies;
        std::unordered_map<ResourceID, std::unordered_set<ResourceID>> m_ReverseDependencies;
        std::unordered_set<ResourceID> m_Dirty;
    };

}