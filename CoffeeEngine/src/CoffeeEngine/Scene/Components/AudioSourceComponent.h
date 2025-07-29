#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Audio/Audio.h"

#include <cereal/cereal.hpp>
#include <glm/mat4x4.hpp>

namespace Coffee
{
}

namespace Coffee
{
    struct AudioSourceComponent
    {
        uint64_t gameObjectID = 0;      ///< The object ID.
        Ref<Audio::AudioBank> audioBank; ///< The audio bank.
        std::string audioBankName;       ///< The name of the audio bank.
        std::string eventName;           ///< The name of the event.
        float volume = 1.f;              ///< The volume of the audio source.
        bool mute = false;               ///< True if the audio source is muted.
        bool playOnAwake = false;        ///< True if the audio source should play automatically.
        glm::mat4 transform;             ///< The transform of the audio source.
        bool isPlaying = false;          ///< True if the audio source is playing.
        bool isPaused = false;           ///< True if the audio source is paused.

        AudioSourceComponent() = default;

        AudioSourceComponent(const AudioSourceComponent& other) { *this = other; }

        static AudioSourceComponent CreateCopy(const AudioSourceComponent& other)
        {
            AudioSourceComponent newComp;
            newComp.audioBank = other.audioBank;
            newComp.audioBankName = other.audioBankName;
            newComp.eventName = other.eventName;
            newComp.volume = other.volume;
            newComp.mute = other.mute;
            newComp.playOnAwake = other.playOnAwake;
            newComp.transform = other.transform;

            return newComp;
        }

        void SetVolume(float volumen)
        {
            if (volumen > 1)
            {
                volumen = 1;
            }
            else if (volumen < 0)
            {
                volumen = 0;
            }
            volume = volumen;
            Audio::SetVolume(this->gameObjectID, this->volume);
        }

        void Play() { Audio::PlayEvent(*this); }
        void Stop() { Audio::StopEvent(*this); }

        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;
        template <class Archive> void load(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::AudioSourceComponent, 0);
