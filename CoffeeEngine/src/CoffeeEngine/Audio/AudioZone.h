#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace Coffee
{
    struct AudioZoneComponent;

    class AudioZone
    {
    public:
        /**
         * @brief Shuts down the audio zone.
         */
        static void Shutdown();

        /**
         * @brief Creates a zone.
         * @param audioZone The audio zone component.
         */
        static void CreateZone(AudioZoneComponent& audioZone);

        /**
         * @brief Updates a reverb zone.
         * @param audioZoneComponent The audio zone component.
         */
        static void UpdateReverbZone(const AudioZoneComponent& audioZoneComponent);

        /**
         * @brief Removes a reverb zone.
         * @param audioZoneComponent The audio zone component.
         */
        static void RemoveReverbZone(const AudioZoneComponent& audioZoneComponent);

        /**
         * @brief Registers an object.
         * @param objectID The object ID.
         * @param position The position.
         */
        static void RegisterObject(const uint64_t objectID, const glm::vec3& position);

        /**
         * @brief Unregisters an object.
         * @param objectID The object ID.
         */
        static void UnregisterObject(const uint64_t objectID);

        /**
         * @brief Updates an object's position.
         * @param objectID The object ID.
         * @param position The position.
         */
        static void UpdateObjectPosition(const uint64_t objectID, const glm::vec3& position);

        /**
         * @brief Updates the audio zone.
         */
        static void Update();

        /**
         * @brief Available bus channels.
         */
        static std::vector<std::string> busNames;

        /**
         * @brief Searches for available bus channels.
         * @return True if successful, false otherwise.
         */
        static bool SearchAvailableBusChannels();

    private:

        /**
         * @brief Enabled effect level.
         */
        static constexpr float EFFECT_LEVEL_ENABLED = 1.0f;

        /**
         * @brief Disabled effect level.
         */
        static constexpr float EFFECT_LEVEL_DISABLED = 0.0f;

        /**
         * @brief Cleans up a zone.
         * @param zoneID The zone ID.
         */
        static void CleanupZone(const uint64_t zoneID);

        /**
         * @param objectPos Object position.
         * @param audioZoneComponent Audio zone component.
         * @return True if the object is in the zone, false otherwise.
         */
        static bool IsObjectInZone(const glm::vec3& objectPos, const AudioZoneComponent& audioZoneComponent);

        /**
         * @brief Sets an object in a reverb zone.
         * @param objectID The object ID.
         * @param audioZones The audio zones.
         */
        static void SetObjectInReverbZone(const uint64_t objectID, const std::vector<AudioZoneComponent*>& audioZones);
    };
}
