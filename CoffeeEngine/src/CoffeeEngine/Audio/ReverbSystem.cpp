#include "ReverbSystem.h"
#include <cassert>

namespace Coffee 
{
    ReverbSystem::ReverbSystem() 
        : m_nextZoneID(1000)
    {
    }

    ReverbSystem::~ReverbSystem()
    {
        Shutdown();
    }

    bool ReverbSystem::Initialize()
    {
        // Obtener el ID del bus auxiliar "MediumRoomVerb" que creaste en Wwise
        m_mediumRoomBusID = AK::SoundEngine::GetIDFromString("MediumRoomVerb");
        return true;
    }

    void ReverbSystem::Shutdown()
    {
        // Limpiar todas las zonas
        for (const auto& zone : m_zones)
        {
            CleanupZone(zone.first);
        }
        m_zones.clear();
    }

    AkGameObjectID ReverbSystem::CreateMediumRoomZone(const AkVector& position, float radius)
    {
        AkGameObjectID zoneID = m_nextZoneID++;

        // Registrar el objeto de juego para la zona de reverb
        AKRESULT result = AK::SoundEngine::RegisterGameObj(zoneID, "MediumRoomReverbZone");
        if (result != AK_Success)
        {
            return AK_INVALID_GAME_OBJECT;
        }

        // Configurar la zona
        ReverbZoneParams params;
        params.position = position;
        params.radius = radius;

        // Guardar los parámetros de la zona
        m_zones[zoneID] = params;

        // Configurar el envío al bus de reverb
        AK::SoundEngine::SetGameObjectOutputBusVolume(zoneID, m_mediumRoomBusID, REVERB_LEVEL_ENABLED);

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
        float level = isInZone ? REVERB_LEVEL_ENABLED : REVERB_LEVEL_DISABLED;
        AK::SoundEngine::SetGameObjectOutputBusVolume(objectID, m_mediumRoomBusID, level);
    }

    void ReverbSystem::CleanupZone(AkGameObjectID zoneID)
    {
        AK::SoundEngine::UnregisterGameObj(zoneID);
    }
}