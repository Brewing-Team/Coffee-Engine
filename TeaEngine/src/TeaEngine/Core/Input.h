#pragma once

#include "TeaEngine/Core/KeyCodes.h"
#include "TeaEngine/Core/MouseCodes.h"

#include <glm/glm.hpp>

namespace Tea {
    
	class Input
	{
	public:
		static bool IsKeyPressed(const KeyCode key);

		static bool IsMouseButtonPressed(const MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}