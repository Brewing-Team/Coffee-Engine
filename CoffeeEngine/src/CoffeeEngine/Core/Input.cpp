#include "CoffeeEngine/Core/Input.h"

#include "CoffeeEngine/Events/ControllerEvent.h"
#include "CoffeeEngine/Events/Event.h"
#include "CoffeeEngine/Events/KeyEvent.h"
#include "CoffeeEngine/Events/MouseEvent.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_mouse.h"

#include <SDL3/SDL_init.h>
#include <glm/gtc/constants.hpp>

namespace Coffee {

    std::vector<InputBinding> Input::m_Bindings = std::vector<InputBinding>(ActionsEnum::ActionCount);
    std::vector<Ref<Gamepad>> Input::m_Gamepads;
    std::unordered_map<ButtonCode, char> Input::m_ButtonStates;
    std::unordered_map<AxisCode, float> Input::m_AxisStates;
    std::unordered_map<AxisCode, float> Input::m_AxisDeadzones;

    void Input::Init()
    {
        SDL_InitSubSystem(SDL_INIT_GAMEPAD);

        //Defaults
        m_AxisDeadzones[Axis::LeftTrigger] = 0.15f;
        m_AxisDeadzones[Axis::RightTrigger] = 0.15f;
        m_AxisDeadzones[Axis::LeftX] = 0.15f;
        m_AxisDeadzones[Axis::RightY] = 0.15f;
        m_AxisDeadzones[Axis::LeftX] = 0.15f;
        m_AxisDeadzones[Axis::RightY] = 0.15f;
    }


     bool Input::IsKeyPressed(const KeyCode key)
    {
        const bool* state = SDL_GetKeyboardState(nullptr);

        return state[key];
    }

    bool Input::IsMouseButtonPressed(const MouseCode button)
    {
        return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_MASK(button);
    }

    glm::vec2 Input::GetMousePosition()
    {
        float x, y;
        SDL_GetMouseState(&x, &y);

        return {x, y};
    }

    float Input::GetMouseX()
    {
        return GetMousePosition().x;
    }

    float Input::GetMouseY()
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
    InputBinding& Input::GetBinding(InputAction action)
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

        if (std::abs(normalizedValue) < deadzone)
        {
            normalizedValue = 0.0f;
        }
       
        m_AxisStates[e.Axis] = normalizedValue;
    }
    void Input::OnKeyPressed(const KeyPressedEvent& kEvent) {
    }

    void Input::OnKeyReleased(const KeyReleasedEvent& kEvent) {
    }

    void Input::OnMouseButtonPressed(const MouseButtonPressedEvent& mEvent) {
    }

    void Input::OnMouseButtonReleased(const MouseButtonReleasedEvent& mEvent) {
    }

    void Input::OnMouseMoved(const MouseMovedEvent& mEvent) {
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