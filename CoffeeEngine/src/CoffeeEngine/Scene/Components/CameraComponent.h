#pragma once

#include "CoffeeEngine/Scene/SceneCamera.h"
#include <cereal/cereal.hpp>

namespace Coffee
{
    /**
     * @brief Component representing a camera.
     * @ingroup scene
     */
    struct CameraComponent
    {
        SceneCamera Camera; ///< The scene camera.

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        /**
         * @brief Serializes the CameraComponent.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template <class Archive> void serialize(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::CameraComponent, 0);
