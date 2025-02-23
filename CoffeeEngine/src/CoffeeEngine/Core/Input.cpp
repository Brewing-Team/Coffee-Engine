#include "CoffeeEngine/Core/Input.h"

#include "CoffeeEngine/Events/ControllerEvent.h"
#include "CoffeeEngine/Events/Event.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_mouse.h"

#include <SDL3/SDL_init.h>
#include <glm/gtc/constants.hpp>

namespace Coffee {

	InputLayer Input::CurrentInputContext = InputLayer::None;

    // Action list initialized to amount of actions
    std::vector<InputBinding> Input::m_bindings = std::vector<InputBinding>(static_cast<int>(InputAction::ActionCount));
    std::vector<Ref<Gamepad>> Input::m_gamepads = std::vector<Ref<Gamepad>>();
    Ref<Gamepad> Input::m_activeController = nullptr;

    void Input::Init()
    {
        SDL_InitSubSystem(SDL_INIT_GAMEPAD);

        COFFEE_CORE_INFO("Input initialized");
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

		return { x, y };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
    {
        return GetMousePosition().y;
    }

    void Input::OnAddController(ControllerAddEvent* cEvent)
    {
        m_gamepads.emplace_back(new Gamepad(cEvent->Controller));
    }

    void Input::OnRemoveController(ControllerRemoveEvent* c_event)
    {
        // Remove controller by SDL_Gamepad ID
        auto pred = [&c_event](Ref<Gamepad> gamepad) {
            return gamepad->getId() == c_event->Controller;
        };
        erase_if(m_gamepads, pred);
    }

    void Input::OnEvent(Event& e)
	{
	    if (e.IsInCategory(EventCategoryInput))
	    {
            switch (e.GetEventType())
            {
            	using enum EventType;
                case KeyPressed:
                {
                    break;
                }
                case KeyReleased:
                {
                    break;
                }
                case ButtonPressed:
                {
                    break;
                }
                case ButtonReleased:
                {
                    break;
                }
                case MouseButtonPressed:
                {
                    break;
                }
                case MouseButtonReleased:
                {
                    break;
                }
                case MouseMoved:
                {
                    break;
                }
                case MouseScrolled:
                {
                    break;
                }
                case ControllerConnected:
                {
                    ControllerAddEvent* cEvent = static_cast<ControllerAddEvent*>(&e);
                    if (cEvent)
                        OnAddController(cEvent);
                    break;
                }
				case ControllerDisconnected:
				{
                    ControllerRemoveEvent* cEvent = static_cast<ControllerRemoveEvent*>(&e);
                    if (cEvent)
                        OnRemoveController(cEvent);
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