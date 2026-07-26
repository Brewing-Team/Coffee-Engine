#include "CoffeeEngine/Core/Input.h"

#include "CoffeeEngine/Core/Application.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Core/Window.h"
#include "CoffeeEngine/Events/Event.h"
#include "CoffeeEngine/Events/KeyEvent.h"
#include "CoffeeEngine/Events/MouseEvent.h"
#include "CoffeeEngine/Project/Project.h"
#include "SDL3/SDL_mouse.h"

#include <SDL3/SDL_init.h>

#include <SDL3/SDL_timer.h>
#include <cereal/archives/json.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>

namespace Coffee
{

Input::Input(const Window& window) : m_Window(window)
{
    m_keys.fill(KeyState::Idle);
    m_mouseButtons.fill(KeyState::Idle);

    m_mousePosition = glm::vec2(0.0f);
    m_mouseWheelDelta = glm::vec2(0.0f);
}
void Input::Update()
{
    m_mouseWheelDelta = glm::vec2(0.0f);

    for (KeyState& keyState : m_keys)
    {
        keyState = (keyState == KeyState::Down || keyState == KeyState::Repeat) ? KeyState::Repeat : KeyState::Idle;
    }

    for (KeyState& buttonState : m_mouseButtons)
    {
        buttonState = (buttonState == KeyState::Down || buttonState == KeyState::Repeat) ? KeyState::Repeat : KeyState::Idle;
    }
}
bool Input::IsKeyJustPressed(const KeyCode key)
{
    return m_keys[key] == KeyState::Down;
}

bool Input::IsKeyPressed(const KeyCode key)
{
    return m_keys[key] == KeyState::Down || m_keys[key] == KeyState::Repeat;
}
bool Input::IsKeyReleased(const KeyCode key)
{
    return m_keys[key] == KeyState::Up;
}
bool Input::IsMouseButtonJustPressed(const MouseCode button)
{
    return m_mouseButtons[button] == KeyState::Down;
}

bool Input::IsMouseButtonPressed(const MouseCode button)
{
    return m_mouseButtons[button] == KeyState::Down || m_mouseButtons[button] == KeyState::Repeat;
}
bool Input::IsMouseButtonReleased(const MouseCode button)
{
    return m_mouseButtons[button] == KeyState::Up;
}
void Input::SetMouseMode(const Window& window, MouseMode mouseMode)
{
    // TODO: Could be nice to handle SDL errors but I'm feeling lazy right now.

    switch (mouseMode)
    {
        using enum MouseMode;
        case MouseMode::Visible:
        {
            SDL_SetWindowRelativeMouseMode(window.GetNativeWindow(), false);
            SDL_SetWindowMouseRect(window.GetNativeWindow(), nullptr);
            SDL_ShowCursor();
            break;
        }
        case MouseMode::Hidden:
        {
            SDL_SetWindowRelativeMouseMode(window.GetNativeWindow(), false);
            SDL_SetWindowMouseRect(window.GetNativeWindow(), nullptr);
            SDL_HideCursor();
            break;
        }
        case MouseMode::Captured:
        {
            SDL_RaiseWindow(window.GetNativeWindow());
            SDL_SetWindowRelativeMouseMode(window.GetNativeWindow(), true);
            SDL_SetWindowMouseRect(window.GetNativeWindow(), nullptr);
            break;
        }
        case MouseMode::Confined:
        {
            SDL_SetWindowRelativeMouseMode(window.GetNativeWindow(), false);
            SDL_ShowCursor();

            int width, height;
            SDL_GetWindowSize(window.GetNativeWindow(), &width, &height);
            SDL_Rect rect = { 0, 0, width, height };

            SDL_SetWindowMouseRect(window.GetNativeWindow(), &rect);
            break;
        }
        case MouseMode::ConfinedHidden:
        {
            SDL_SetWindowRelativeMouseMode(window.GetNativeWindow(), false);
            SDL_HideCursor();

            int width, height;
            SDL_GetWindowSize(window.GetNativeWindow(), &width, &height);
            SDL_Rect rect = { 0, 0, width, height };

            SDL_SetWindowMouseRect(window.GetNativeWindow(), &rect);
            break;
        }
    }

    m_mouseMode = mouseMode;
}

const glm::vec2& Input::GetMousePosition()
{
    return m_mousePosition;
}

glm::vec2 Input::GetMouseDelta()
{
    glm::vec2 delta;
    SDL_GetRelativeMouseState(&delta.x, &delta.y);
    return delta;
}
} // namespace Coffee