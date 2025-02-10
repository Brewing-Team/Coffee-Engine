#include "Audio.h"

#include "CoffeeEngine/Scene/Components.h"
#include "ReverbSystem.h"

#include <cassert>
#include <fstream>
#include <sstream>

namespace Coffee
{

    // Global pointer for the low-level IO
    CAkFilePackageLowLevelIODeferred* g_lowLevelIO = nullptr;

    glm::vec3 frontSoundObjectPos(0.0f, 0.0f, 100.0f);
    glm::vec3 backSoundObjectPos(0.0f, 0.0f, -100.0f);

    float frontSpeed = 50.0f;
    float backSpeed = 50.0f;
    bool frontMovingRight = true;
    bool backMovingUp = true;

    glm::vec3 listenerPos(0.0f, 0.0f, 0.0f);
    glm::vec3 forward(0.0f, 0.0f, 1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    float listenerSpeed = 50.0f;
    bool listenerMovingForward = true;

    std::vector<Audio::AudioBank*> Audio::audioBanks;
    std::vector<AudioSourceComponent*> Audio::audioSources;

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

        LoadAudioBanks();

        // Set the listener
        AkGameObjectID listenerID = 200;
        RegisterGameObject(100);
        ReverbSystem::Initialize();

        RegisterGameObject(listenerID);

        AK::SoundEngine::SetDefaultListeners(&listenerID, 1);
        ReverbSystem::Update();

        PlayEvent("Play_test_sound", 100);

        PlayEvent("Play_test_sound", 100);

        AkGameObjectID leftSoundObject = 300;
        AkGameObjectID rightSoundObject = 301;
        AkGameObjectID frontSoundObject = 302;
        AkGameObjectID backSoundObject = 303;

        RegisterGameObject(leftSoundObject);
        RegisterGameObject(rightSoundObject);
        RegisterGameObject(frontSoundObject);
        RegisterGameObject(backSoundObject);

        Play3DSound("Play_test_sound", leftSoundObject, -2000.0f, 0.0f, 0.0f);
        Play3DSound("Play_test_sound2", rightSoundObject, 2000.0f, 0.0f, 0.0f);
        Play3DSound("Play_test_sound3", frontSoundObject, 0.0f, 0.0f, 2000.0f);
        Play3DSound("Play_test_sound4", backSoundObject, 0.0f, 0.0f, -2000.0f);
    }

    void Audio::RegisterGameObject(AkGameObjectID gameObjectID)
    {
        AK::SoundEngine::RegisterGameObj(gameObjectID);
    }

    void Audio::Set3DPosition(AkGameObjectID gameObjectID, glm::vec3& pos, glm::vec3& forward, glm::vec3& up)
    {
        AkSoundPosition newPos;
        newPos.SetPosition(pos.x, pos.y, pos.z);
        newPos.SetOrientation(forward.x, forward.y, forward.z, up.x, up.y, up.z);

        AK::SoundEngine::SetPosition(gameObjectID, newPos);
    }

    void Audio::Play3DSound(const char* eventName, AkGameObjectID gameObjectID, float x, float y, float z)
    {
        glm::vec3 position(x, y, z);
        glm::vec3 forward(0.0f, 0.0f, 1.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);

        Set3DPosition(gameObjectID, position, forward, up);

        PlayEvent(eventName, gameObjectID);
    }

    void Audio::SetListenerPosition(glm::vec3& pos, glm::vec3& forward, glm::vec3& up)
    {
        AkSoundPosition listenerPos;
        listenerPos.SetPosition(pos.x, pos.y, pos.z);
        listenerPos.SetOrientation(forward.x, forward.y, forward.z, up.x, up.y, up.z);

        AkGameObjectID listenerID = 200; // ID del listener registrado
        AK::SoundEngine::SetPosition(listenerID, listenerPos);
    }

    void Audio::PlayEvent(const char* eventName, AkGameObjectID gameObjectID)
    {
        AK::SoundEngine::PostEvent(eventName, gameObjectID);
    }

    void Audio::SetSwitch(const char* switchGroup, const char* switchState, AkGameObjectID gameObjectID)
    {
        AK::SoundEngine::SetSwitch(switchGroup, switchState, gameObjectID);
    }

    void Audio::SetVolume(AkGameObjectID gameObjectID, float newVolume)
    {
        AK::SoundEngine::SetGameObjectOutputBusVolume(gameObjectID, AK_INVALID_GAME_OBJECT, newVolume);
    }

    void Audio::RegisterAudioSourceComponent(AudioSourceComponent& audioSourceComponent)
    {
        audioSources.push_back(&audioSourceComponent);
        audioSourceComponent.gameObjectID = audioSources.size();

        RegisterGameObject(audioSourceComponent.gameObjectID);
    }

    void Audio::ProcessAudio()
    {
        ReverbSystem::Update();
      
        static float elapsedTime = 0.0f;
        elapsedTime += 0.001f;

        if (frontMovingRight)
            frontSoundObjectPos.x += frontSpeed * elapsedTime;
        else
            frontSoundObjectPos.x -= frontSpeed * elapsedTime;

        if (frontSoundObjectPos.x > 200.0f)
            frontMovingRight = false;
        else if (frontSoundObjectPos.x < -200.0f)
            frontMovingRight = true;

        if (backMovingUp)
            backSoundObjectPos.y += backSpeed * elapsedTime;
        else
            backSoundObjectPos.y -= backSpeed * elapsedTime;

        if (backSoundObjectPos.y > 200.0f)
            backMovingUp = false;
        else if (backSoundObjectPos.y < -200.0f)
            backMovingUp = true;

        glm::vec3 forward(0.0f, 0.0f, 1.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);

        Set3DPosition(302, frontSoundObjectPos, forward, up);
        Set3DPosition(303, backSoundObjectPos, forward, up);

        AK::SoundEngine::RenderAudio();

        if (listenerMovingForward)
            listenerPos.z += listenerSpeed * elapsedTime;
        else
            listenerPos.z -= listenerSpeed * elapsedTime;

        if (listenerPos.z > 200.0f)
            listenerMovingForward = false;
        else if (listenerPos.z < -200.0f)
            listenerMovingForward = true;

        SetListenerPosition(listenerPos, forward, up);

        elapsedTime = 0.0f;
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

    bool Audio::LoadAudioBanks()
    {
        std::ifstream file("Assets\\Audio\\Wwise Project\\GeneratedSoundBanks\\Windows\\SoundbanksInfo.json");
        if (!file.is_open())
            return false;

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        rapidjson::Document banksInfo;
        if (banksInfo.Parse(buffer.str().c_str()).HasParseError()
            || !banksInfo.HasMember("SoundBanksInfo")
            || !banksInfo["SoundBanksInfo"].HasMember("SoundBanks"))
        {
            return false;
        }

        const auto& soundBanks = banksInfo["SoundBanksInfo"]["SoundBanks"];
        if (!soundBanks.IsArray())
            return false;

        for (const auto& bankData : soundBanks.GetArray())
        {
            if (!bankData.HasMember("ShortName") || !bankData["ShortName"].IsString())
                continue;

            const std::string shortName = bankData["ShortName"].GetString();

            auto audioBank = new AudioBank();
            audioBank->name = shortName;

            if (bankData.HasMember("Events") && bankData["Events"].IsArray())
            {
                for (const auto& eventData : bankData["Events"].GetArray())
                {
                    if (eventData.HasMember("Name") && eventData["Name"].IsString())
                    {
                        std::string eventName = eventData["Name"].GetString();
                        audioBank->events.push_back(eventName);
                    }
                }
            }

            AkBankID bankID;
            AK::SoundEngine::LoadBank(audioBank->name.c_str(), bankID);

            audioBanks.push_back(audioBank);
        }

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
