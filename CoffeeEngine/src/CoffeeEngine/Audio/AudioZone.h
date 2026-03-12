#pragma once

#include <glm/fwd.hpp>
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
        void Shutdown();

        /**
         * @brief Creates a zone.
         * @param audioZone The audio zone component.
         */
        void CreateZone(AudioZoneComponent& audioZone);

        /**
         * @brief Updates a reverb zone.
         * @param audioZoneComponent The audio zone component.
         */
        void UpdateReverbZone(const AudioZoneComponent& audioZoneComponent);

        /**
         * @brief Removes a reverb zone.
         * @param audioZoneComponent The audio zone component.
         */
        void RemoveReverbZone(const AudioZoneComponent& audioZoneComponent);

        /**
         * @brief Removes all reverb zones.
         */
        void RemoveAllReverbZones();

        /**
         * @brief Registers an object.
         * @param objectID The object ID.
         * @param position The position.
         */
        void RegisterObject(const uint64_t objectID, const glm::vec3& position);

        /**
         * @brief Unregisters an object.
         * @param objectID The object ID.
         */
        void UnregisterObject(const uint64_t objectID);

        /**
         * @brief Updates an object's position.
         * @param objectID The object ID.
         * @param position The position.
         */
        void UpdateObjectPosition(const uint64_t objectID, const glm::vec3& position);

        /**
         * @brief Updates the audio zone.
         */
        void Update();

        /**
         * @brief Available bus channels.
         */
        std::vector<std::string> busNames;

        /**
         * @brief Searches for available bus channels.
         * @return True if successful, false otherwise.
         */
        bool SearchAvailableBusChannels();

    private:

        /**
         * @brief Cleans up a zone.
         * @param zoneID The zone ID.
         */
        void CleanupZone(const uint64_t zoneID);

        /**
         * @param objectPos Object position.
         * @param audioZoneComponent Audio zone component.
         * @return True if the object is in the zone, false otherwise.
         */
        bool IsObjectInZone(const glm::vec3& objectPos, const AudioZoneComponent& audioZoneComponent);

        /**
         * @brief Sets an object in a reverb zone.
         * @param objectID The object ID.
         * @param audioZones The audio zones.
         */
        void SetObjectInReverbZone(const uint64_t objectID, const std::vector<AudioZoneComponent*>& audioZones);
    private:
        uint64_t m_nextZoneID= 1000;
        std::unordered_map<uint64_t, AudioZoneComponent*> m_zones;
        std::unordered_map<uint64_t, glm::vec3> m_registeredObjects;
        std::vector<std::string> m_busNames;
    };
}
