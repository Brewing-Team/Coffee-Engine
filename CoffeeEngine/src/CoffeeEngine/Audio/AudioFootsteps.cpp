#include "AudioFootsteps.h"
#include "CoffeeEngine/Renderer/EditorCamera.h"

#include <SDL3/SDL_keyboard.h>

namespace Coffee {

    glm::vec3 m_ListenerPosition; // Listener's current position
    AkGameObjectID m_ListenerID = 150;  // ID for the audio listener
    const char* m_CurrentSurface; // Current surface type
    bool m_IsPlaying;             // Whether a sound is already playing

    void AudioFootsteps::Initialize()
    {
        // Register the listener with Wwise
        Audio::RegisterGameObject(m_ListenerID);
        AK::SoundEngine::SetDefaultListeners(&m_ListenerID, 2);
    }

    void AudioFootsteps::Update() {
        // Handle input and update position
        HandleInput();

        // Update the listener position in Wwise
        AkSoundPosition soundPosition;
        soundPosition.SetPosition(m_ListenerPosition.x, m_ListenerPosition.y, m_ListenerPosition.z);
        soundPosition.SetOrientation(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f); // Default orientation
        AK::SoundEngine::SetPosition(m_ListenerID, soundPosition);

        // Determine surface and play appropriate sound
        DetermineSurface();
    }

    bool AudioFootsteps::IsKeyPressed(int key)
    {
        const bool* state = SDL_GetKeyboardState(nullptr);

        return state[key];
    }
    void AudioFootsteps::HandleInput() {
        // Update position based on key presses
        if (AudioFootsteps::IsKeyPressed(24)) m_ListenerPosition.z += 1.0f; // Forward
        if (AudioFootsteps::IsKeyPressed(13)) m_ListenerPosition.z -= 1.0f; // Backward
        if (AudioFootsteps::IsKeyPressed(11)) m_ListenerPosition.x -= 1.0f; // Left
        if (AudioFootsteps::IsKeyPressed(14)) m_ListenerPosition.x += 1.0f; // Right
    }

    void AudioFootsteps::DetermineSurface() {
        const char* newSurface;

        // Define grid-based positions for different surfaces
        if (m_ListenerPosition.x > 0.0f && m_ListenerPosition.z > 0.0f ) {
            newSurface = "Water";
        } else if (m_ListenerPosition.x > 0.0f && m_ListenerPosition.z < 0.0f) {
            newSurface = "Grass";
        } else if (m_ListenerPosition.x < 0.0f && m_ListenerPosition.z > 0.0f) {
            newSurface = "Mud";
        } else if (m_ListenerPosition.x < 0.0f && m_ListenerPosition.z < 0.0f) {
            newSurface = "Boots";
        } else {
            newSurface = "Water";
        }

        printf("%s\n", newSurface);
        printf("%f\n", m_ListenerPosition.x);

        // If the surface changes, stop the current sound and play the new one
        if (newSurface != m_CurrentSurface) {
            StopLoopingSound();
            m_CurrentSurface = newSurface;
            StartLoopingSound();
        }
    }

    void AudioFootsteps::StartLoopingSound() {
        if (!m_IsPlaying) {
            Audio::SetSwitch("SurfaceType", m_CurrentSurface, m_ListenerID);
            Audio::PlayEvent("Foot_Player", m_ListenerID);
            m_IsPlaying = true;
        }
    }

    void AudioFootsteps::StopLoopingSound() {
        if (m_IsPlaying) {
            Audio::PlayEvent("Foot_Player_Stop", m_ListenerID);
            m_IsPlaying = false;
        }
    }

}
