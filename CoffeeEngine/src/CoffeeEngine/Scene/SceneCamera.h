#pragma once

#include "CoffeeEngine/Renderer/Camera.h"
#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>

namespace Coffee {

    /**
     * @defgroup scene Scene
     * @{
     */

    /**
     * @brief Camera class for the scene.
     * @ingroup scene
     */
    class SceneCamera : public Camera
    {
    public:
        /**
         * @brief Constructor for SceneCamera.
         */
        SceneCamera();

        /**
         * @brief Default destructor.
         */
        ~SceneCamera() = default;
    private:
        friend class cereal::access;

        /**
         * @brief Serializes the SceneCamera.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::base_class<Camera>(this));
        }
    };

    /** @} */ // end of scene group

}

CEREAL_REGISTER_TYPE(Coffee::SceneCamera);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Camera, Coffee::SceneCamera);