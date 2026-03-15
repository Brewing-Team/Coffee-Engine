/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include "CoffeeEngine/Resources/Resource.h"
#include <unordered_map>

namespace Coffee {

    //TODO: Update the Resource Registry to use the path as key to avoid collisions.

    /**
     * @class ResourceRegistry
     * @brief Manages the registration and retrieval of resources.
     */
    class ResourceRegistry
    {
    public:
        /**
         * @brief Adds a resource to the registry.
         * @param name The name of the resource.
         * @param resource A reference to the resource to add.
         */
        void Add(ResourceID uuid, ResourceRef<Resource> resource)
        { 
            m_Resources[uuid] = resource;

            const std::string& name = resource->GetName();
            m_NameToUUID[name] = uuid;
        }

        /**
         * @brief Retrieves a resource from the registry.
         * @tparam T The type of the resource.
         * @param name The name of the resource.
         * @return A reference to the resource, or nullptr if not found.
         */
        template<typename T>
        ResourceRef<T> Get(ResourceID uuid)
        {
            if (!Exists(uuid))
            {
                COFFEE_CORE_ERROR("Resource {0} not found!", (uint64_t)uuid);
                return nullptr;
            }
            return std::static_pointer_cast<T>(m_Resources[uuid]);
        }

        /**
         * @brief Retrieves a resource from the registry.
         * @param name The name of the resource.
         * @return A reference to the resource, or nullptr if not found.
         */
         template<typename T>
        ResourceRef<T> Get(const std::string& name)
        {
            if (!Exists(name))
            {
                COFFEE_CORE_ERROR("Resource {0} not found!", name);
                return nullptr;
            }
            return std::static_pointer_cast<T>(m_Resources[m_NameToUUID[name]]);
        }

        /**
         * @brief Checks if a resource exists in the registry.
         * @param name The name of the resource.
         * @return True if the resource exists, false otherwise.
         */
        bool Exists(ResourceID uuid) { return m_Resources.find(uuid) != m_Resources.end(); }

        /**
         * @brief Checks if a resource exists in the registry.
         * @param name The name of the resource.
         * @return True if the resource exists, false otherwise.
         */
        bool Exists(const std::string& name) { return m_NameToUUID.find(name) != m_NameToUUID.end(); }

        void Remove(ResourceID uuid)
        {
            if (Exists(uuid))
            {
                m_NameToUUID.erase(m_Resources[uuid]->GetName());
                m_Resources.erase(uuid);
            }
        }

        /**
         * @brief Clears all resources from the registry.
         */
        void Clear() 
        {
            m_Resources.clear();
            m_NameToUUID.clear();
        }

        ResourceID GetUUIDByName(const std::string& name) { return m_NameToUUID[name]; }

        /**
         * @brief Gets the entire resource registry.
         * @return A constant reference to the resource registry.
         */
        const std::unordered_map<ResourceID, ResourceRef<Resource>>& GetResourceRegistry() const { return m_Resources; }

    private:
        std::unordered_map<ResourceID, ResourceRef<Resource>> m_Resources; ///< The resource registry.
        std::unordered_map<std::string, ResourceID> m_NameToUUID; ///< The mapping of resource names to UUIDs.
    };

}

/** @} */