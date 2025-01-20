#include "Audio.h"

#include <cassert>

#include <AK/IBytes.h>
#include <AK/SoundEngine/Common/AkTypes.h>

#include <AK/MusicEngine/Common/AkMusicEngine.h>     // Music Engine
#include <AK/Plugin/AllPluginsFactories.h>           // Plugin factories
#include <AK/SoundEngine/Common/AkMemoryMgr.h>       // Memory Manager interface

#include <AK/SoundEngine/Common/AkSoundEngine.h>     // Sound Engine
#include <AK/SoundEngine/Common/AkStreamMgrModule.h> // Stream Manager
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>   // Spatial Audio

#if AK_WWISESDK_VERSION_MAJOR < 2024
#include <AK/SoundEngine/Common/AkModule.h> // Default memory manager
#else
#include <AK/SoundEngine/Common/AkMemoryMgrModule.h> // Default memory manager
#endif

#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h> // Communication (for debug builds)
#endif

#include <AkFilePackageLowLevelIODeferred.h> // File I/O

namespace Coffee
{

    // Global pointer for the low-level IO
    CAkFilePackageLowLevelIODeferred* g_lowLevelIO = nullptr;

    void Audio::Init()
    {
        if (!InitializeMemoryManager())
            return;

        if (!InitializeStreamManager())
            return;

        if (!InitializeLowLevelIO())
            return;

        if (!InitializeSoundEngine())
            return;

        if (!InitializeMusicEngine())
            return;

        if (!InitializeSpatialAudio())
            return;
    }

    void Audio::ProcessAudio()
    {
        AK::SoundEngine::RenderAudio();
    }

    bool Audio::InitializeMemoryManager()
    {
        AkMemSettings memSettings;
        AK::MemoryMgr::GetDefaultSettings(memSettings);

        if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
        {
            assert(!"Could not create the Memory Manager.");
            return false;
        }
        return true;
    }

    bool Audio::InitializeStreamManager()
    {
        AkStreamMgrSettings stmSettings;
        AK::StreamMgr::GetDefaultSettings(stmSettings);

        if (!AK::StreamMgr::Create(stmSettings))
        {
            assert(!"Could not create the Stream Manager.");
            return false;
        }
        return true;
    }

    bool Audio::InitializeLowLevelIO()
    {
        AkDeviceSettings deviceSettings;
        AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

        g_lowLevelIO = new CAkFilePackageLowLevelIODeferred();
        if (g_lowLevelIO->Init(deviceSettings) != AK_Success)
        {
            assert(!"Could not initialize the Low-Level IO.");
            return false;
        }
        return true;
    }

    bool Audio::InitializeSoundEngine()
    {
        AkInitSettings initSettings;
        AkPlatformInitSettings platformInitSettings;
        AK::SoundEngine::GetDefaultInitSettings(initSettings);
        AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

        if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
        {
            assert(!"Could not initialize the Sound Engine.");
            return false;
        }
        return true;
    }

    bool Audio::InitializeMusicEngine()
    {
        AkMusicSettings musicInitSettings;
        AK::MusicEngine::GetDefaultInitSettings(musicInitSettings);

        if (AK::MusicEngine::Init(&musicInitSettings) != AK_Success)
        {
            assert(!"Could not initialize the Music Engine.");
            return false;
        }
        return true;
    }

    bool Audio::InitializeSpatialAudio()
    {
        AkSpatialAudioInitSettings spatialAudioSettings;
        if (AK::SpatialAudio::Init(spatialAudioSettings) != AK_Success)
        {
            assert(!"Could not initialize the Spatial Audio module.");
            return false;
        }
        return true;
    }

#ifndef AK_OPTIMIZED
    bool Audio::InitializeCommunicationModule()
    {
        AkCommSettings commSettings;
        AK::Comm::GetDefaultInitSettings(commSettings);

        if (AK::Comm::Init(commSettings) != AK_Success)
        {
            assert(!"Could not initialize the Communication module.");
            return false;
        }
        return true;
    }
#endif // AK_OPTIMIZED

    void Audio::Shutdown()
    {
#ifndef AK_OPTIMIZED
        // Terminate the Communication module
        AK::Comm::Term();
#endif // AK_OPTIMIZED

        // Terminate the Music Engine
        AK::MusicEngine::Term();

        // Terminate the Sound Engine
        AK::SoundEngine::Term();

        // Destroy the Stream Manager and terminate the low-level IO
        if (AK::IAkStreamMgr::Get())
        {
            g_lowLevelIO->Term();
            delete g_lowLevelIO;
            AK::IAkStreamMgr::Get()->Destroy();
        }

        // Terminate the Memory Manager
        AK::MemoryMgr::Term();
    }
} // namespace Coffee
