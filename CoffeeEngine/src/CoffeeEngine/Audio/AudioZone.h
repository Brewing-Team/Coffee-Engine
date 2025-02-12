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
        AudioZone();
        ~AudioZone();

        static void Shutdown();

        static void CreateZone(AudioZoneComponent& audioZone);

        static void UpdateReverbZone(const AudioZoneComponent& audioZoneComponent);

        static void RemoveReverbZone(const AudioZoneComponent& audioZoneComponent);

        static void SetObjectInReverbZone(const uint64_t objectID, const std::vector<AudioZoneComponent*>& audioZones);

        static void RegisterObject(const uint64_t objectID, const glm::vec3& position);

        static void UnregisterObject(const uint64_t objectID);

        static void UpdateObjectPosition(const uint64_t objectID, const glm::vec3& position);

        static void Update();

        static std::vector<std::string> busNames;

        static bool SearchAvailableBusChannels();

    private:

        static constexpr float EFFECT_LEVEL_ENABLED = 1.0f;
        static constexpr float EFFECT_LEVEL_DISABLED = 0.0f;

        static void CleanupZone(uint64_t zoneID);
        static bool IsObjectInZone(const glm::vec3& objectPos, const AudioZoneComponent& audioZoneComponent);
    };
}
