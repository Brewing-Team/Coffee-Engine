#pragma once

#include "CoffeeEngine/Renderer/Camera.h"

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
        glm::mat4 m_ViewMatrix; ///< The view matrix of the camera.
    };

    /** @} */ // end of scene group

}