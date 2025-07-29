#pragma once

#include <cereal/cereal.hpp>
#include <glm/mat4x4.hpp>

namespace Coffee
{
    struct AudioListenerComponent
    {
        uint64_t gameObjectID = 0; ///< The object ID.
        glm::mat4 transform;        ///< The transform of the audio listener.

        AudioListenerComponent() = default;

        AudioListenerComponent(const AudioListenerComponent& other) { *this = other; }

        static AudioListenerComponent CreateCopy(const AudioListenerComponent& other)
        {
            AudioListenerComponent newComp;
            newComp.transform = other.transform;

            return newComp;
        }

        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;
        template <class Archive> void load(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::AudioListenerComponent, 0);
