#pragma once

#include "NavMeshComponent.h"
#include "CoffeeEngine/Core/Base.h"

#include <cereal/cereal.hpp>

namespace Coffee {

    class NavMeshPathfinding;

}

namespace Coffee
{
    struct NavigationAgentComponent
    {
        std::vector<glm::vec3> Path; ///< The path to follow.
        bool ShowDebug = false;      ///< Flag to show the navigation agent debug.

        /**
         * @brief Finds a path from the start to the end.
         * @param start The start position.
         * @param end The end position.
         * @return The path.
         */
        std::vector<glm::vec3> FindPath(const glm::vec3 start, const glm::vec3 end) const;

        /**
         * @brief Gets the pathfinder.
         * @return The pathfinder.
         */
        Ref<NavMeshPathfinding> GetPathFinder() const { return m_PathFinder; }

        /**
         * @brief Sets the pathfinder.
         * @param pathFinder The pathfinder to set.
         */
        void SetPathFinder(const Ref<NavMeshPathfinding>& pathFinder) { m_PathFinder = pathFinder; }

        /**
         * @brief Gets the navigation mesh component.
         * @return The navigation mesh component.
         */
        Ref<NavMeshComponent> GetNavMeshComponent() const { return m_NavMeshComponent; }

        /**
         * @brief Sets the navigation mesh component.
         * @param navMeshComponent The navigation mesh component to set.
         */
        void SetNavMeshComponent(const Ref<NavMeshComponent>& navMeshComponent)
        {
            m_NavMeshComponent = navMeshComponent;
        }

        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;
        template <class Archive> void load(Archive& archive, std::uint32_t const version);

      private:
        Ref<NavMeshPathfinding> m_PathFinder = nullptr;     ///< The pathfinder.
        Ref<NavMeshComponent> m_NavMeshComponent = nullptr; ///< The navigation mesh component.
    };
}

CEREAL_CLASS_VERSION(Coffee::NavigationAgentComponent, 0);
