#pragma once

#include "TeaEngine/Renderer/Camera.h"

namespace Tea {

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
    };

    /** @} */ // end of scene group

}