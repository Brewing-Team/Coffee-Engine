#include "CoffeeEngine/Core/Input.h"

#include "CoffeeEngine/Events/Event.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_mouse.h"

#include <glm/gtc/constants.hpp>

namespace Coffee {

	InputLayer Input::CurrentInputContext = InputLayer::None;
    std::vector<InputBinding> m_bindings = std::vector<InputBinding>(static_cast<int>(InputAction::ActionCount));
    Ref<SDL_Gamepad> Input::m_controller = nullptr;

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
                    break;
                }
				case ControllerDisconnected:
				{
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