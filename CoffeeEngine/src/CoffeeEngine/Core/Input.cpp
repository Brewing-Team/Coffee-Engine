#include "CoffeeEngine/Core/Input.h"

#include "CoffeeEngine/Events/ControllerEvent.h"
#include "CoffeeEngine/Events/Event.h"
#include "CoffeeEngine/Events/KeyEvent.h"
#include "CoffeeEngine/Events/MouseEvent.h"
#include "CoffeeEngine/Project/Project.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_mouse.h"

#include <SDL3/SDL_init.h>

#include <cereal/types/vector.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>

namespace Coffee {

    constexpr const char* MAPPING_FILE_PATH = "cfg/InputMapping.json";

    std::vector<InputBinding> Input::m_Bindings = std::vector<InputBinding>(ActionsEnum::ActionCount);
    std::vector<Ref<Gamepad>> Input::m_Gamepads;
    std::unordered_map<ButtonCode, uint8_t> Input::m_ButtonStates = {{Button::Invalid, 0}};
    std::unordered_map<AxisCode, float> Input::m_AxisStates = {{Axis::Invalid, 0.0f}};
    std::unordered_map<AxisCode, float> Input::m_AxisDeadzones;
    std::unordered_map<KeyCode, bool> Input::m_KeyStates = {{Key::Unknown, false}};
    std::unordered_map<MouseCode, bool> Input::m_MouseStates;
    glm::vec2 Input::m_MousePosition = glm::vec2(0.0f);

    void Input::Init()
    {
        SDL_InitSubSystem(SDL_INIT_GAMEPAD);

        #pragma region Defaults

        // Axis deadzone defaults
        m_AxisDeadzones[Axis::LeftTrigger] = 0.15f;
        m_AxisDeadzones[Axis::RightTrigger] = 0.15f;
        m_AxisDeadzones[Axis::LeftX] = 0.15f;
        m_AxisDeadzones[Axis::RightX] = 0.15f;
        m_AxisDeadzones[Axis::LeftY] = 0.15f;
        m_AxisDeadzones[Axis::RightY] = 0.15f;

        //UI defaults
        m_Bindings[ActionsEnum::UiMoveHorizontal].SetName("UiMoveHorizontal").SetAxis(Axis::LeftX).SetButtonNeg(Button::DpadLeft).SetButtonPos(Button::DpadRight);
        m_Bindings[ActionsEnum::UiMoveVertical].SetName("UiMoveVertical").SetAxis(Axis::LeftY).SetButtonNeg(Button::DpadDown).SetButtonPos(Button::DpadUp);
        m_Bindings[ActionsEnum::Cancel].SetName("Cancel").SetButtonPos(Button::East);
        m_Bindings[ActionsEnum::Confirm].SetName("Confirm").SetButtonPos(Button::South);

        // Gameplay defaults
        m_Bindings[ActionsEnum::MoveHorizontal].SetName("MoveHorizontal").SetAxis(Axis::LeftX);
        m_Bindings[ActionsEnum::MoveVertical].SetName("MoveVertical").SetAxis(Axis::LeftY);
        m_Bindings[ActionsEnum::AimHorizontal].SetName("AimHorizontal").SetAxis(Axis::RightX);
        m_Bindings[ActionsEnum::AimVertical].SetName("AimVertical").SetAxis(Axis::RightY);
        m_Bindings[ActionsEnum::Shoot].SetName("Shoot").SetAxis(Axis::RightTrigger);
        m_Bindings[ActionsEnum::Melee].SetName("Melee").SetButtonPos(Button::RightShoulder);
        m_Bindings[ActionsEnum::Interact].SetName("Interact").SetButtonPos(Button::South);
        m_Bindings[ActionsEnum::Dash].SetName("Dash").SetButtonPos(Button::East);
        m_Bindings[ActionsEnum::Cover].SetName("Cover").SetButtonPos(Button::West);
        m_Bindings[ActionsEnum::Skill1].SetName("Skill1").SetButtonPos(Button::North);
        m_Bindings[ActionsEnum::Skill2].SetName("Skill2").SetButtonPos(Button::LeftShoulder);
        m_Bindings[ActionsEnum::Skill3].SetName("Skill3").SetAxis(Axis::LeftTrigger);
        m_Bindings[ActionsEnum::Injector].SetName("Injector").SetButtonPos(Button::DpadUp);
        m_Bindings[ActionsEnum::Grenade].SetName("Grenade").SetButtonPos(Button::DpadRight);
        m_Bindings[ActionsEnum::Map].SetName("Map").SetButtonPos(Button::Back);
        m_Bindings[ActionsEnum::Pause].SetName("Pause").SetButtonPos(Button::Start);

        #pragma endregion

    }
    void Input::Save()
    {
        // Can't save project input mapping if there's no project
        if (Project::GetActive() == nullptr) return;

        auto path = Project::GetProjectDirectory() / MAPPING_FILE_PATH;

        std::ofstream file(path);

        cereal::JSONOutputArchive archive(file);

        archive(m_Bindings);
    }

    void Input::Load()
    {

    }

    bool Input::IsKeyPressed(const KeyCode key)
    {
        return m_KeyStates[key];
        //const bool* state = SDL_GetKeyboardState(nullptr);
        //return state[key];
    }

    bool Input::IsMouseButtonPressed(const MouseCode button)
    {
        return m_MouseStates[button];
        // return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_MASK(button);
    }

    const glm::vec2& Input::GetMousePosition()
    {
        return m_MousePosition;
        // float x, y;
        // SDL_GetMouseState(&x, &y);
        // return {x, y};
    }

    const float Input::GetMouseX()
    {
        return GetMousePosition().x;
    }

    const float Input::GetMouseY()
    {
        return GetMousePosition().y;
    }

    bool Input::GetButtonRaw(const ButtonCode button)
    {
        return m_ButtonStates[button];
    }

    float Input::GetAxisRaw(const AxisCode axis)
    {
        return m_AxisStates[axis];
    }
    InputBinding& Input::GetBinding(const InputAction action)
    {
        return m_Bindings[action];
    }

    void Input::OnAddController(const ControllerAddEvent* cEvent)
    {
        m_Gamepads.emplace_back(new Gamepad(cEvent->Controller));
    }


    void Input::OnRemoveController(const ControllerRemoveEvent* cEvent)
    {
        // Remove controller by SDL_Gamepad ID
        auto pred = [&cEvent](const Ref<Gamepad>& gamepad) {
            return gamepad->getId() == cEvent->Controller;
        };
        erase_if(m_Gamepads, pred);
    }
    void Input::OnButtonPressed(const ButtonPressEvent& e) {
        m_ButtonStates[e.Button] += 1;
    }

    void Input::OnButtonReleased(const ButtonReleaseEvent& e) {
        m_ButtonStates[e.Button] -= 1;
    }

    void Input::OnAxisMoved(const AxisMoveEvent& e) {

        float deadzone = m_AxisDeadzones[e.Axis];
        float normalizedValue = e.Value / 32767.0f;

        // TODO fix axis curve (currently starts at whatever the deadzone value is instead of at slightly larger than 0)
        if (std::abs(normalizedValue) < deadzone)
        {
            normalizedValue = 0.0f;
        }
       
        m_AxisStates[e.Axis] = normalizedValue;
    }
    void Input::OnKeyPressed(const KeyPressedEvent& kEvent) {
        m_KeyStates[kEvent.GetKeyCode()] = true;
    }

    void Input::OnKeyReleased(const KeyReleasedEvent& kEvent) {
        m_KeyStates[kEvent.GetKeyCode()] = false;
    }

    void Input::OnMouseButtonPressed(const MouseButtonPressedEvent& mEvent) {
        m_MouseStates[mEvent.GetMouseButton()] = true;
    }

    void Input::OnMouseButtonReleased(const MouseButtonReleasedEvent& mEvent) {
        m_MouseStates[mEvent.GetMouseButton()] = false;
    }

    void Input::OnMouseMoved(const MouseMovedEvent& mEvent) {
        m_MousePosition.x = mEvent.GetX();
        m_MousePosition.y = mEvent.GetY();
    }

    void Input::OnEvent(Event& e)
	{
        //TODO change this code for an event dispatcher
	    if (e.IsInCategory(EventCategoryInput))
	    {
            switch (e.GetEventType())
            {
            	using enum EventType;
                case ControllerConnected:
                {
                if (const auto* cEvent = static_cast<ControllerAddEvent*>(&e))
                        OnAddController(cEvent);
                    break;
                }
                case ControllerDisconnected:
                {
                    if (const auto* cEvent = static_cast<ControllerRemoveEvent*>(&e))
                        OnRemoveController(cEvent);
                    break;
                }
                case ButtonPressed:
                {
                    if (const auto* bEvent = static_cast<ButtonPressEvent*>(&e))
                        OnButtonPressed(*bEvent);
                    break;
                }
                case ButtonReleased:
                {
                    if (const auto* bEvent = static_cast<ButtonReleaseEvent*>(&e))
                        OnButtonReleased(*bEvent);
                    break;
                }
                case AxisMoved:
                {
                    if (const auto* aEvent = static_cast<AxisMoveEvent*>(&e))
                        OnAxisMoved(*aEvent);
                    break;
                }
                case KeyPressed:
                {
                    if (const auto* kEvent = static_cast<KeyPressedEvent*>(&e))
                        OnKeyPressed(*kEvent);
                    break;
                }
                case KeyReleased:
                {
                    if (const auto* kEvent = static_cast<KeyReleasedEvent*>(&e))
                        OnKeyReleased(*kEvent);
                    break;
                }
                case MouseButtonPressed:
                {
                    if (const auto* mEvent = static_cast<MouseButtonPressedEvent*>(&e))
                        OnMouseButtonPressed(*mEvent);
                    break;
                }
                case MouseButtonReleased:
                {
                    if (const auto* mEvent = static_cast<MouseButtonReleasedEvent*>(&e))
                        OnMouseButtonReleased(*mEvent);
                    break;
                }
                case MouseMoved:
                {
                    if (const auto* mEvent = static_cast<MouseMovedEvent*>(&e))
                        OnMouseMoved(*mEvent);
                    break;
                }

                default:
                {
                    break;
                }
            }
	    }
	}

}