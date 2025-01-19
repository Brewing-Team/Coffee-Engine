#include "Audio.h"

#include "assert.h"

#include "AK/IBytes.h"
#include <AK/SoundEngine/Common/AkTypes.h>

#include "AK/SoundEngine/Common/AkMemoryMgr.h"			// Memory Manager interface
#include <AK/SoundEngine/Common/AkMemoryMgrModule.h>
#include "AK/SoundEngine/Common/AkStreamMgrModule.h"

#include "AK/SoundEngine/Common/IAkStreamMgr.h"			// Streaming Manager
#include "AK/Tools/Common/AkPlatformFuncs.h"			// Thread defines

#include "AK/SoundEngine/Common/AkSoundEngine.h"		// Sound engine

#include "AK/MusicEngine/Common/AkMusicEngine.h"		// Music Engine

#include "AK/SpatialAudio/Common/AkSpatialAudio.h"

#include "AkFilePackageLowLevelIODeferred.h"


#include <AK/Plugin/AllPluginsFactories.h>

#ifndef AK_OPTIMIZED
#include "AK/Comm/AkCommunication.h"
#endif

CAkFilePackageLowLevelIODeferred* g_lowLevelIO;

namespace Coffee {

    void Audio::Init()
    {
        AkMemSettings memSettings;
        AK::MemoryMgr::GetDefaultSettings(memSettings);

        if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
        {
            assert(!"Could not create the memory manager.");
        }

        AkStreamMgrSettings stmSettings;
        AK::StreamMgr::GetDefaultSettings(stmSettings);

        // Customize the Stream Manager settings here.

        if (!AK::StreamMgr::Create(stmSettings))
        {
            assert(!"Could not create the Streaming Manager");
        }

        //
        // Create a streaming device.
        // Note that you can override the default low-level I/O module with your own.
        //
        AkDeviceSettings deviceSettings;
        AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

        g_lowLevelIO = new CAkFilePackageLowLevelIODeferred();


        // Customize the streaming device settings here.

        // CAkFilePackageLowLevelIODeferred::Init() creates a streaming device
        // in the Stream Manager, and registers itself as the File Location Resolver.

        AkInitSettings initSettings;
        AkPlatformInitSettings platformInitSettings;
        AK::SoundEngine::GetDefaultInitSettings(initSettings);
        AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

        if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
        {
            assert(!"Could not initialize the Sound Engine.");
        }

        AkMusicSettings musicInit;
        AK::MusicEngine::GetDefaultInitSettings(musicInit);

        if (AK::MusicEngine::Init(&musicInit) != AK_Success)
        {
            assert(!"Could not initialize the Music Engine.");
        }

        AkSpatialAudioInitSettings settings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings.
        if (AK::SpatialAudio::Init(settings) != AK_Success)
        {
            assert(!"Could not initialize the Spatial Audio.");
        }

#ifndef AK_OPTIMIZED
        AkCommSettings commSettings;
        AK::Comm::GetDefaultInitSettings(commSettings);
        if (AK::Comm::Init(commSettings) != AK_Success)
        {
            assert(!"Could not initialize communication.");
        }
#endif // AK_OPTIMIZED
    }

    void Audio::Shutdown()
    {
        AK::Comm::Term();
        //AK::SpatialAudio::Term();
        AK::MusicEngine::Term();
        AK::SoundEngine::Term();


        if ( AK::IAkStreamMgr::Get() )
            AK::IAkStreamMgr::Get()->Destroy();

        AK::MemoryMgr::Term();
    }

}