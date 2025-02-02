#pragma once
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <unordered_map>
#include <string>

namespace Coffee
{
    struct ReverbZoneParams
    {
        AkVector position;
        float radius;
    };

    class ReverbSystem
    {
    public:
        ReverbSystem();
        ~ReverbSystem();

        // Inicialización y cierre
        bool Initialize();
        void Shutdown();

        // Crear una zona de reverb específica para MediumRoomVerb
        AkGameObjectID CreateMediumRoomZone(const AkVector& position, float radius);

        // Actualizar posición y tamaño de la zona
        void UpdateReverbZone(AkGameObjectID zoneID, const AkVector& position, float radius);

        // Eliminar una zona
        void RemoveReverbZone(AkGameObjectID zoneID);

        // Manejar objetos entrando/saliendo de la zona
        void SetObjectInReverbZone(AkGameObjectID objectID, bool isInZone);

    private:
        AkGameObjectID m_nextZoneID;
        AkAuxBusID m_mediumRoomBusID;
        std::unordered_map<AkGameObjectID, ReverbZoneParams> m_zones;
        
        // Nivel de efecto de reverb
        static constexpr float REVERB_LEVEL_ENABLED = 1.0f;
        static constexpr float REVERB_LEVEL_DISABLED = 0.0f;

        void CleanupZone(AkGameObjectID zoneID);
    };
}