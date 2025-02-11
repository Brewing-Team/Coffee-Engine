#include "ReverbSystem.h"

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <unordered_map>

#include <AK/Plugin/AkRoomVerbFXFactory.h>

namespace Coffee
{
    AkGameObjectID m_nextZoneID= 1000;
    AkAuxBusID m_mediumRoomBusID;
    std::unordered_map<AkGameObjectID, ReverbZoneParams> m_zones;

    std::unordered_map<AkGameObjectID, AkVector> m_registeredObjects;

    ReverbSystem::~ReverbSystem()
    {
        Shutdown();
    }

    bool ReverbSystem::Initialize()
    {
        m_mediumRoomBusID = AK::SoundEngine::GetIDFromString("MediumRoomVerb");
        CreateMediumRoomZone({0,0,0},20);
        return m_mediumRoomBusID != AK_INVALID_UNIQUE_ID;
    }

    void ReverbSystem::Shutdown()
    {
        for (const auto& zone : m_zones)
        {
            CleanupZone(zone.first);
        }
        m_zones.clear();
        m_registeredObjects.clear();
    }

    AkGameObjectID ReverbSystem::CreateMediumRoomZone(const AkVector& position, float radius)
    {
        AkGameObjectID zoneID = m_nextZoneID++;

        AKRESULT result = AK::SoundEngine::RegisterGameObj(zoneID, "MediumRoomReverbZone");
        if (result != AK_Success)
        {
            return AK_INVALID_GAME_OBJECT;
        }

        ReverbZoneParams params;
        params.position = position;
        params.radius = radius;
        m_zones[zoneID] = params;

        return zoneID;
    }

    void ReverbSystem::UpdateReverbZone(AkGameObjectID zoneID, const AkVector& position, float radius)
    {
        auto it = m_zones.find(zoneID);
        if (it != m_zones.end())
        {
            it->second.position = position;
            it->second.radius = radius;
        }
    }

    void ReverbSystem::RemoveReverbZone(AkGameObjectID zoneID)
    {
        CleanupZone(zoneID);
        m_zones.erase(zoneID);
    }

    void ReverbSystem::SetObjectInReverbZone(AkGameObjectID objectID, bool isInZone)
    {
        AkAuxSendValue auxSend;
        auxSend.listenerID = AK_INVALID_GAME_OBJECT;
        auxSend.auxBusID = m_mediumRoomBusID;
        auxSend.fControlValue = isInZone ? REVERB_LEVEL_ENABLED : REVERB_LEVEL_DISABLED;

        AK::SoundEngine::SetGameObjectAuxSendValues(objectID, &auxSend, 1);
    }

    void ReverbSystem::RegisterObject(AkGameObjectID objectID, const AkVector& position)
    {
        m_registeredObjects[objectID] = position;
    }

    void ReverbSystem::UnregisterObject(AkGameObjectID objectID)
    {
        m_registeredObjects.erase(objectID);
    }

    void ReverbSystem::UpdateObjectPosition(AkGameObjectID objectID, const AkVector& position)
    {
        m_registeredObjects[objectID] = position;
    }

    void ReverbSystem::Update()
    {
        for (const auto& zone : m_zones)
        {
            for (auto& object : m_registeredObjects)
            {
                bool isInZone = IsObjectInZone(object.second, zone.second);
                SetObjectInReverbZone(object.first, isInZone);
            }
        }
    }

    void ReverbSystem::CleanupZone(AkGameObjectID zoneID)
    {
        AK::SoundEngine::UnregisterGameObj(zoneID);
    }

    bool ReverbSystem::IsObjectInZone(const AkVector& objectPos, const ReverbZoneParams& zoneParams)
    {
        float dx = objectPos.X - zoneParams.position.X;
        float dy = objectPos.Y - zoneParams.position.Y;
        float dz = objectPos.Z - zoneParams.position.Z;
        float distanceSquared = dx * dx + dy * dy + dz * dz;
        return distanceSquared <= (zoneParams.radius * zoneParams.radius);
    }
}
