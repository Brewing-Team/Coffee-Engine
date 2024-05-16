#include "Input.h"

namespace Tea {

	bool Input::IsKeyPressed(const SDL_Scancode key)
	{
		const Uint8* state = SDL_GetKeyboardState(NULL); // Passing NULL for size parameter
        return state[key];
	}

	bool Input::IsMouseButtonPressed(const Uint8 button)
	{
		return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button);
	}

	glm::vec2 Input::GetMousePosition()
	{
        int x, y;
		SDL_GetMouseState(&x, &y);

		return { (float)x, (float)y };
	}

}