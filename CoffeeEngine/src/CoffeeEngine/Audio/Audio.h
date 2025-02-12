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

#include "CoffeeEngine/Core/Base.h"
#include "AudioZone.h"

#include <AkFilePackageLowLevelIODeferred.h> // File I/O

#include <glm/vec3.hpp>
#include <vector>
#include <string>

namespace Coffee {

    struct AudioSourceComponent;
    struct AudioListenerComponent;

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
        static void PlayEvent(const char* eventName, uint64_t gameObjectID);

        /**
         * @brief Stops an event.
         * @param eventName The name of the event.
         * @param gameObjectID The game object ID.
         */
        static void StopEvent(const char* eventName, uint64_t gameObjectID);

        /**
         * @brief Pauses an event.
         * @param eventName The name of the event.
         * @param gameObjectID The game object ID.
         */
        static void PauseEvent(const char* eventName, uint64_t gameObjectID);

        /**
         * @brief Resumes an event.
         * @param eventName The name of the event.
         * @param gameObjectID The game object ID.
         */
        static void ResumeEvent(const char* eventName, uint64_t gameObjectID);

        /**
         * @brief Sets a switch.
         * @param switchGroup The name of the switch group.
         * @param switchState The name of the switch state.
         * @param gameObjectID The game object ID.
         */
        static void SetSwitch(const char* switchGroup, const char* switchState, uint64_t gameObjectID);

        /**
         * @brief Sets the position of a game object.
         * @param gameObjectID The game object ID.
         * @param pos Position.
         * @param forward Forward.
         * @param up Up.
         */
        static void Set3DPosition(uint64_t gameObjectID, glm::vec3 pos, glm::vec3 forward, glm::vec3 up);

        /**
         * @brief Register a game object.
         * @param gameObjectID The game object ID.
         */
        static void RegisterGameObject(uint64_t gameObjectID);

        /**
         * @brief Unregister a game object.
         * @param gameObjectID The game object ID.
         */
        static void UnregisterGameObject(uint64_t gameObjectID);

        /**
         * @brief Audio bank.
         */
        struct AudioBank
        {
            std::string name;
            std::vector<std::string> events;
        };

        /**
         * @brief Audio banks.
         */
        static std::vector<Ref<AudioBank>> audioBanks;

        /**
         * @brief Set the volume of a game object.
         * @param gameObjectID The game object ID.
         * @param newVolume The new volume.
         */
        static void SetVolume(uint64_t gameObjectID, float newVolume);

        /**
         * @brief Audio source components.
         */
        static std::vector<AudioSourceComponent*> audioSources;

        /**
         * @brief Register an audio source component.
         * @param audioSourceComponent The audio source component.
         */
        static void RegisterAudioSourceComponent(AudioSourceComponent& audioSourceComponent);

        /**
         * @brief Unregister an audio source component.
         * @param audioSourceComponent The audio source component.
         */
        static void UnregisterAudioSourceComponent(AudioSourceComponent& audioSourceComponent);

        /**
         * @brief Audio listener components.
         */
        static std::vector<AudioListenerComponent*> audioListeners;

        /**
         * @brief Register an audio listener component.
         * @param audioListenerComponent The audio listener component.
         */
        static void RegisterAudioListenerComponent(AudioListenerComponent& audioListenerComponent);

        /**
         * @brief Unregister an audio listener component.
         * @param audioListenerComponent The audio listener component.
         */
        static void UnregisterAudioListenerComponent(AudioListenerComponent& audioListenerComponent);

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

        /**
         * @brief Loads audio banks.
         * @return True if successful, false otherwise.
         */
        static bool LoadAudioBanks();
    };

}