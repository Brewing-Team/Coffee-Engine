#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/UUID.h"
#include <cereal/cereal.hpp>

namespace Coffee
{
    // Forward declaration
    struct AnimatorComponent;
    class Mesh;

    /**
     * @brief Component representing a mesh.
     * @ingroup scene
     */
    struct MeshComponent
    {
        Ref<Mesh> mesh;        ///< The mesh reference.
        bool drawAABB = false; ///< Flag to draw the axis-aligned bounding box (AABB).

        AnimatorComponent* animator = nullptr; ///< The animator component.
        UUID animatorUUID = 0;                 ///< The UUID of the animator.

        MeshComponent() {}
        MeshComponent(const MeshComponent&) = default;
        MeshComponent(Ref<Mesh> mesh) : mesh(mesh) {}
        ~MeshComponent() { animator = nullptr; }

        /**
         * @brief Gets the mesh reference.
         * @return The mesh reference.
         */
        const Ref<Mesh>& GetMesh() const { return mesh; }

      private:
        friend class cereal::access;
        /**
         * @brief Serializes the MeshComponent.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;
        template <class Archive> void load(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::MeshComponent, 0);
