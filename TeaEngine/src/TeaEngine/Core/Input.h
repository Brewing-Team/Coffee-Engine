#ifndef INPUT_H
#define INPUT_H

#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace Tea {
    
	class Input
	{
	public:
		static bool IsKeyPressed(SDL_Scancode key);

		static bool IsMouseButtonPressed(Uint8 button);
		static glm::vec2 GetMousePosition();
	};
}

#endif