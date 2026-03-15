#include "ResourceDependencyGraph.h"

namespace Coffee {

    void ResourceDependencyGraph::Clear()
    {
        m_Dependencies.clear();
        m_ReverseDependencies.clear();
        m_Dirty.clear();
    }

    void ResourceDependencyGraph::RemoveResource(ResourceID id)
    {
        auto depIt = m_Dependencies.find(id);
        if (depIt != m_Dependencies.end())
        {
            for (ResourceID dependency : depIt->second)
            {
                m_ReverseDependencies[dependency].erase(id);
            }
            m_Dependencies.erase(depIt);
        }

        auto revIt = m_ReverseDependencies.find(id);
        if (revIt != m_ReverseDependencies.end())
        {
            for (ResourceID dependent : revIt->second)
            {
                m_Dependencies[dependent].erase(id);
            }
            m_ReverseDependencies.erase(revIt);
        }

        m_Dirty.erase(id);
    }

    void ResourceDependencyGraph::AddDependency(ResourceID owner, ResourceID dependency)
    {
        m_Dependencies[owner].insert(dependency);
        m_ReverseDependencies[dependency].insert(owner);
    }

    std::vector<ResourceID> ResourceDependencyGraph::GetDependencies(ResourceID owner) const
    {
        auto it = m_Dependencies.find(owner);
        if (it == m_Dependencies.end())
            return {};

        return {it->second.begin(), it->second.end()};
    }

    std::vector<ResourceID> ResourceDependencyGraph::GetDependents(ResourceID dependency) const
    {
        auto it = m_ReverseDependencies.find(dependency);
        if (it == m_ReverseDependencies.end())
            return {};

        return {it->second.begin(), it->second.end()};
    }

    void ResourceDependencyGraph::MarkDirty(ResourceID id)
    {
        m_Dirty.insert(id);
    }

    bool ResourceDependencyGraph::IsDirty(ResourceID id) const
    {
        return m_Dirty.find(id) != m_Dirty.end();
    }

    void ResourceDependencyGraph::ClearDirty(ResourceID id)
    {
        m_Dirty.erase(id);
    }

}