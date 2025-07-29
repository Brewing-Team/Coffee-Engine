#pragma once

#include "CoffeeEngine/Audio/AudioZone.h"
#include <cereal/cereal.hpp>
#include <glm/vec3.hpp>

namespace Coffee
{
    struct AudioZoneComponent
    {
        uint64_t zoneID = -1;                 ///< The zone ID.
        std::string audioBusName;             ///< The name of the audio bus.
        glm::vec3 position = {0.f, 0.f, 0.f}; ///< The position of the audio zone.
        float radius = 1.f;                   ///< The radius of the audio zone.

        AudioZoneComponent() = default;

        AudioZoneComponent(const AudioZoneComponent& other) { *this = other; }

        AudioZoneComponent& operator=(const AudioZoneComponent& other)
        {
            if (this != &other)
            {
                zoneID = other.zoneID;
                audioBusName = other.audioBusName;
                position = other.position;
                radius = other.radius;

                AudioZone::CreateZone(*this);
            }
            return *this;
        }

        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;
        template <class Archive> void load(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::AudioZoneComponent, 0);
