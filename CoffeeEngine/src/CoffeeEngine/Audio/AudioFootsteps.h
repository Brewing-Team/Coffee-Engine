#pragma once

#include "AK/SoundEngine/Common/AkTypes.h"
#include <glm/glm.hpp> // For position handling


namespace Coffee {

    class AudioFootsteps {

    public:
        /**
         * @brief Constructor for AudioFootsteps.
         * @param listenerID The Wwise Game Object ID to represent the audio listener.
         */

        static bool IsKeyPressed(int key);

        explicit AudioFootsteps(AkGameObjectID listenerID);

        /**
         * @brief Initializes and registers the audio listener.
         */
        static void Initialize();

        /**
         * @brief Updates the listener's position and plays the corresponding sound.
         */
        static void Update();

        /**
         * @brief Starts looping sound based on the current surface.
         */
        static void StartLoopingSound();

        /**
         * @brief Stops the currently playing sound.
         */
        static void StopLoopingSound();

    private:
        /**
         * @brief Processes input to update the listener's position.
         */
        static void HandleInput();

        /**
         * @brief Determines the current surface based on the listener's position.
         */
        static void DetermineSurface();

    };

}
