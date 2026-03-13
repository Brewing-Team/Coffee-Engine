#pragma once

#include "CoffeeEngine/Core/Base.h"

#include <cereal/cereal.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <string>
#include <filesystem>

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

        const std::filesystem::path DefaultAudioPath = std::filesystem::absolute(std::filesystem::current_path() / "assets/audio/Wwise Project/GeneratedSoundBanks/Windows");

        /**
         * @brief Initializes the audio system.
         */
        Audio();

        /**
         * @brief Shuts down the audio system.
         */
        void Shutdown();

        /**
         * @brief Processes audio.
         */
        void ProcessAudio();

        /**
         * @brief Plays an event.
         * @param audioSourceComponent The audio source component.
         */
        void PlayEvent(AudioSourceComponent& audioSourceComponent);

        /**
         * @brief Stops an event.
         * @param audioSourceComponent The audio source component.
         */
        void StopEvent(AudioSourceComponent& audioSourceComponent);

        /**
         * @brief Pauses an event.
         * @param audioSourceComponent The audio source component.
         */
        void PauseEvent(AudioSourceComponent& audioSourceComponent);

        /**
         * @brief Resumes an event.
         * @param audioSourceComponent The audio source component.
         */
        void ResumeEvent(AudioSourceComponent& audioSourceComponent);

        /**
         * @brief Sets a switch.
         * @param switchGroup The name of the switch group.
         * @param switchState The name of the switch state.
         * @param gameObjectID The game object ID.
         */
        void SetSwitch(const char* switchGroup, const char* switchState, uint64_t gameObjectID);

        /**
         * @brief Sets the position of a game object.
         * @param gameObjectID The game object ID.
         * @param pos Position.
         * @param forward Forward.
         * @param up Up.
         */
        void Set3DPosition(uint64_t gameObjectID, glm::vec3 pos, glm::vec3 forward, glm::vec3 up);

        /**
         * @brief Register a game object.
         * @param gameObjectID The game object ID.
         */
        void RegisterGameObject(uint64_t gameObjectID);

        /**
         * @brief Unregister a game object.
         * @param gameObjectID The game object ID.
         */
        void UnregisterGameObject(uint64_t gameObjectID);

        /**
         * @brief Unregister all game objects.
         */
        void UnregisterAllGameObjects();

        /**
         * @brief Audio bank.
         */
        struct AudioBank
        {
            std::string name; ///< The name of the bank.
            std::vector<std::string> events; ///< The events of the bank.

            /**
             * @brief Serializes the AudioBank.
             * @tparam Archive The type of the archive.
             * @param archive The archive to serialize to.
             */
            template<class Archive> void serialize(Archive& archive, std::uint32_t const version);
        };
        
        
        /**
         * @brief Audio banks.
         */
        std::vector<Ref<AudioBank>> audioBanks;

        /**
         * @brief Set the volume of a game object.
         * @param gameObjectID The game object ID.
         * @param newVolume The new volume.
         */
        void SetVolume(uint64_t gameObjectID, float newVolume);

        /**
         * @brief Audio source components.
         */
        std::vector<AudioSourceComponent*> audioSources;

        /**
         * @brief Register an audio source component.
         * @param audioSourceComponent The audio source component.
         */
        void RegisterAudioSourceComponent(AudioSourceComponent& audioSourceComponent);

        /**
         * @brief Unregister an audio source component.
         * @param audioSourceComponent The audio source component.
         */
        void UnregisterAudioSourceComponent(AudioSourceComponent& audioSourceComponent);

        /**
         * @brief Audio listener components.
         */
        std::vector<AudioListenerComponent*> audioListeners;

        /**
         * @brief Register an audio listener component.
         * @param audioListenerComponent The audio listener component.
         */
        void RegisterAudioListenerComponent(AudioListenerComponent& audioListenerComponent);

        /**
         * @brief Unregister an audio listener component.
         * @param audioListenerComponent The audio listener component.
         */
        void UnregisterAudioListenerComponent(AudioListenerComponent& audioListenerComponent);

        /**
         * @brief Play the audio sources chosen to play on awake.
         */
        void PlayInitialAudios();

        /**
         * @brief Stop all audio events.
         */
        void StopAllEvents();

        /**
         * @brief Sets the volume of an audio bus.
         * @param busName The name of the bus.
         * @param volume The volume value (0.0 to 1.0).
         */
        void SetBusVolume(const char* busName, float volume);

        const std::filesystem::path& GetAudioPath() { return m_ActiveAudioPath; }

        void OnProjectLoad();

        void OnProjectUnload();

    private:

        std::filesystem::path m_ActiveAudioPath;

        /**
         * @brief Initializes the memory manager.
         * @return True if successful, false otherwise.
         */
        bool InitializeMemoryManager();

        /**
         * @brief Initializes the stream manager.
         * @return True if successful, false otherwise.
         */
        bool InitializeStreamManager();

        /**
         * @brief Initializes low-level I/O.
         * @return True if successful, false otherwise.
         */
        bool InitializeLowLevelIO();

        /**
         * @brief Initializes the sound engine.
         * @return True if successful, false otherwise.
         */
        bool InitializeSoundEngine();

        /**
         * @brief Initializes the music engine.
         * @return True if successful, false otherwise.
         */
        bool InitializeMusicEngine();

        /**
         * @brief Initializes spatial audio.
         * @return True if successful, false otherwise.
         */
        bool InitializeSpatialAudio();

        /**
         * @brief Initializes the communication module.
         * @return True if successful, false otherwise.
         */
        bool InitializeCommunicationModule();

        /**
         * @brief Loads audio banks.
         * @return True if successful, false otherwise.
         */
        bool LoadAudioBanks();

        bool ReloadAudioBanks();
    };
} // namespace Coffee
CEREAL_CLASS_VERSION(Coffee::Audio::AudioBank, 0);
