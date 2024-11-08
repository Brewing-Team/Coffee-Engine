/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"
#include <string>
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
        static void Add(const std::string& name, Ref<Resource> resource) { m_Resources[name] = resource; }

        /**
         * @brief Retrieves a resource from the registry.
         * @tparam T The type of the resource.
         * @param name The name of the resource.
         * @return A reference to the resource, or nullptr if not found.
         */
        template<typename T>
        static Ref<T> Get(const std::string& name)
        {
            if (!Exists(name))
            {
                COFFEE_CORE_ERROR("Resource {0} not found!", name.c_str());
                return nullptr;
            }
            return std::static_pointer_cast<T>(m_Resources[name]);
        }

        /**
         * @brief Checks if a resource exists in the registry.
         * @param name The name of the resource.
         * @return True if the resource exists, false otherwise.
         */
        static bool Exists(const std::string& name) { return m_Resources.find(name) != m_Resources.end(); }

        /**
         * @brief Clears all resources from the registry.
         */
        static void Clear() { m_Resources.clear(); }

        /**
         * @brief Gets the entire resource registry.
         * @return A constant reference to the resource registry.
         */
        static const std::unordered_map<std::string, Ref<Resource>>& GetResourceRegistry() { return m_Resources; }

    private:
        static std::unordered_map<std::string, Ref<Resource>> m_Resources; ///< The resource registry.
    };

}

/** @} */