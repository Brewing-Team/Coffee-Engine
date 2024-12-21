/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/UUID.h"
#include "CoffeeEngine/IO/Resource.h"
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
        static void Add(UUID uuid, Ref<Resource> resource)
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
        static Ref<T> Get(UUID uuid)
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
        static Ref<T> Get(const std::string& name)
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
        static bool Exists(UUID uuid) { return m_Resources.find(uuid) != m_Resources.end(); }

        /**
         * @brief Checks if a resource exists in the registry.
         * @param name The name of the resource.
         * @return True if the resource exists, false otherwise.
         */
        static bool Exists(const std::string& name) { return m_NameToUUID.find(name) != m_NameToUUID.end(); }

        static void Remove(UUID uuid)
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
        static void Clear() 
        {
            m_Resources.clear();
            m_NameToUUID.clear();
        }

        static UUID GetUUIDByName(const std::string& name) { return m_NameToUUID[name]; }

        /**
         * @brief Gets the entire resource registry.
         * @return A constant reference to the resource registry.
         */
        static const std::unordered_map<UUID, Ref<Resource>>& GetResourceRegistry() { return m_Resources; }

    private:
        static std::unordered_map<UUID, Ref<Resource>> m_Resources; ///< The resource registry.
        static std::unordered_map<std::string, UUID> m_NameToUUID; ///< The mapping of resource names to UUIDs.
    };

}

/** @} */