#pragma once

namespace Coffee {

    /**
     * @class Audio
     * @brief Manages audio systems including memory, sound, music, and spatial audio.
     */
    class Audio
    {
    public:

        /**
         * @brief Initializes the audio system.
         */
        static void Init();

        /**
         * @brief Shuts down the audio system.
         */
        static void Shutdown();

        /**
         * @brief Processes audio.
         */
        static void ProcessAudio();

    private:

        /**
         * @brief Initializes the memory manager.
         * @return True if successful, false otherwise.
         */
        static bool InitializeMemoryManager();

        /**
         * @brief Initializes the stream manager.
         * @return True if successful, false otherwise.
         */
        static bool InitializeStreamManager();

        /**
         * @brief Initializes low-level I/O.
         * @return True if successful, false otherwise.
         */
        static bool InitializeLowLevelIO();

        /**
         * @brief Initializes the sound engine.
         * @return True if successful, false otherwise.
         */
        static bool InitializeSoundEngine();

        /**
         * @brief Initializes the music engine.
         * @return True if successful, false otherwise.
         */
        static bool InitializeMusicEngine();

        /**
         * @brief Initializes spatial audio.
         * @return True if successful, false otherwise.
         */
        static bool InitializeSpatialAudio();

#ifndef AK_OPTIMIZED
        /**
         * @brief Initializes the communication module.
         * @return True if successful, false otherwise.
         */
        static bool InitializeCommunicationModule();
#endif // AK_OPTIMIZED

    };

}