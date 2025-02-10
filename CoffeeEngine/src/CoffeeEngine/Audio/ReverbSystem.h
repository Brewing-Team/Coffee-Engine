#pragma once
#include <AK/SoundEngine/Common/AkTypes.h>



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
        static bool Initialize();
       static void Shutdown();

        // Crear una zona de reverb específica para MediumRoomVerb
        static AkGameObjectID CreateMediumRoomZone(const AkVector& position, float radius);

        // Actualizar posición y tamaño de la zona
        static void UpdateReverbZone(AkGameObjectID zoneID, const AkVector& position, float radius);

        // Eliminar una zona
        static void RemoveReverbZone(AkGameObjectID zoneID);

        // Manejar objetos entrando/saliendo de la zona
        static void SetObjectInReverbZone(AkGameObjectID objectID, bool isInZone);

        // Registrar y gestionar objetos
        static void RegisterObject(AkGameObjectID objectID, const AkVector& position);
        static void UnregisterObject(AkGameObjectID objectID);
        static void UpdateObjectPosition(AkGameObjectID objectID, const AkVector& position);

        // Actualizar dinámicamente las zonas de reverb
        static void Update();

    private:


        // Nivel de efecto de reverb
        static constexpr float REVERB_LEVEL_ENABLED = 1.0f;
        static constexpr float REVERB_LEVEL_DISABLED = 0.0f;

        static void CleanupZone(AkGameObjectID zoneID);
        static bool IsObjectInZone(const AkVector& objectPos, const ReverbZoneParams& zoneParams) ;
    };
}
