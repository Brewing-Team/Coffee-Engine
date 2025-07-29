#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/UUID.h"
#include <cereal/cereal.hpp>

namespace Coffee
{
    class NavMesh;

}

namespace Coffee
{
    struct NavMeshComponent
    {
        bool ShowDebug = false; ///< Flag to show the navigation mesh debug.

        /**
         * @brief Gets the navigation mesh.
         * @return The navigation mesh.
         */
        Ref<NavMesh> GetNavMesh() const { return m_NavMesh; }

        /**
         * @brief Sets the navigation mesh.
         * @param navMesh The navigation mesh to set.
         */
        void SetNavMesh(const Ref<NavMesh>& navMesh) { m_NavMesh = navMesh; }

        /**
         * @brief Gets the UUID of the navigation mesh.
         * @return The UUID of the navigation mesh.
         */
        UUID GetNavMeshUUID() const { return m_NavMeshUUID; }

        /**
         * @brief Sets the UUID of the navigation mesh.
         * @param navMeshUUID The UUID of the navigation mesh to set.
         */
        void SetNavMeshUUID(const UUID& navMeshUUID) { m_NavMeshUUID = navMeshUUID; }

        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;
        template <class Archive> void load(Archive& archive, std::uint32_t const version);

      private:
        Ref<NavMesh> m_NavMesh = nullptr; ///< The navigation mesh.
        UUID m_NavMeshUUID;               ///< The UUID of the navigation mesh.
    };
}

CEREAL_CLASS_VERSION(Coffee::NavMeshComponent, 0);
