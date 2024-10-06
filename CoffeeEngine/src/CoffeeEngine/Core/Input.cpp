#include "CoffeeEngine/Core/Input.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_mouse.h"

namespace Coffee {

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

}