#pragma once

namespace Coffee {

    class Scene;
    class ResourceManager;

    class ResourceResolver
    {
    public:
        explicit ResourceResolver(ResourceManager* resourceManager = nullptr)
            : m_ResourceManager(resourceManager)
        {
        }

        void SetResourceManager(ResourceManager* resourceManager) { m_ResourceManager = resourceManager; }

        // NOTE: This should be expanded as components migrate to two-phase Deserialize/Resolve.
        void ResolveSceneResourceReferences(Scene& scene) const;

    private:
        ResourceManager* m_ResourceManager = nullptr;
    };

}