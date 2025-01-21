#include "Audio.h"

#include <cassert>

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

        if (!InitializeCommunicationModule())
            return;

        g_lowLevelIO->SetBasePath(AKTEXT("assets\\audio\\Wwise Project\\GeneratedSoundBanks\\Windows"));

        AkBankID bankID;
        AK::SoundEngine::LoadBank("Init.bnk", bankID);
        AK::SoundEngine::LoadBank("CoffeeEngine.bnk", bankID);

        // Register the game object
        AkGameObjectID gameObjectID = 100;
        AK::SoundEngine::RegisterGameObj(gameObjectID);

        // Set the listener
        AkGameObjectID listenerID = 200;
        AK::SoundEngine::RegisterGameObj(listenerID);
        AK::SoundEngine::SetDefaultListeners(&listenerID, 1);

        Play("Play_test_sound", gameObjectID);
        Play("Play_BackgroundContainer", gameObjectID);
    }

    void Audio::Play(const char* eventName, AkGameObjectID gameObjectID)
    {
        AK::SoundEngine::PostEvent(eventName, gameObjectID);
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

    bool Audio::InitializeCommunicationModule()
    {
#ifndef AK_OPTIMIZED
        AkCommSettings commSettings;
        AK::Comm::GetDefaultInitSettings(commSettings);

        AKPLATFORM::SafeStrCpy(commSettings.szAppNetworkName, "Coffee Engine", AK_COMM_SETTINGS_MAX_STRING_SIZE);

        if (AK::Comm::Init(commSettings) != AK_Success)
        {
            assert(!"Could not initialize the Communication module.");
            return false;
        }
#endif // AK_OPTIMIZED
        return true;
    }

    void Audio::Shutdown()
    {
        // Unload the soundbanks
        AK::SoundEngine::ClearBanks();

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
