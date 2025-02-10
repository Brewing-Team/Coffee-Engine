#pragma once

#include <AK/IBytes.h>
#include <AK/SoundEngine/Common/AkTypes.h>

#include <AK/MusicEngine/Common/AkMusicEngine.h>     // Music Engine
#include <AK/SoundEngine/Common/AkMemoryMgr.h>       // Memory Manager interface

#include <AK/SoundEngine/Common/AkSoundEngine.h>     // Sound Engine
#include <AK/SoundEngine/Common/AkStreamMgrModule.h> // Stream Manager
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>   // Spatial Audio

#if AK_WWISESDK_VERSION_MAJOR < 2024
#include <AK/SoundEngine/Common/AkModule.h> // Default memory manager
#else
#include <AK/SoundEngine/Common/AkMemoryMgrModule.h> // Default memory manager
#endif

#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h> // Communication (for debug builds)
#endif

#include <AkFilePackageLowLevelIODeferred.h> // File I/O

#include <glm/vec3.hpp>
#include <vector>
#include <string>

namespace Coffee {

    struct AudioSourceComponent;

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

        /**
         * @brief Plays an event.
         * @param eventName The name of the event.
         * @param gameObjectID The game object ID.
         */
        static void PlayEvent(const char* eventName, AkGameObjectID gameObjectID);

        static void SetSwitch(const char* switchGroup, const char* switchState, AkGameObjectID gameObjectID);

        static void Play3DSound(const char* eventName, AkGameObjectID gameObjectID, float x, float y, float z);
        static void Set3DPosition(AkGameObjectID gameObjectID, glm::vec3& pos, glm::vec3& forward, glm::vec3& up);
        static void SetListenerPosition(glm::vec3& pos, glm::vec3& forward, glm::vec3& up);

        /**
         * @brief Register a game object.
         * @param gameObjectID The game object ID.
         */
        static void RegisterGameObject(AkGameObjectID gameObjectID);

        struct AudioBank
        {
            std::string name;
            std::vector<std::string> events;
        };

        static std::vector<AudioBank*> audioBanks;

        static void SetVolume(AkGameObjectID gameObjectID, float newVolume);

        static std::vector<AudioSourceComponent*> audioSources;

        static void RegisterAudioSourceComponent(AudioSourceComponent& audioSourceComponent);

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


        static bool LoadAudioBanks();
    };

}