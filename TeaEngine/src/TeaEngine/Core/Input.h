#pragma once

#include "TeaEngine/Core/KeyCodes.h"
#include "TeaEngine/Core/MouseCodes.h"

#include <glm/glm.hpp>

namespace Tea {
    /**
     * @defgroup core Core
     * @{
     */
	class Input
	{
	public:
		/**
		 * Checks if a specific key is currently being pressed.
		 *
		 * @param key The key code of the key to check.
		 * @return True if the key is currently being pressed, false otherwise.
		 */
		static bool IsKeyPressed(const KeyCode key);

		/**
		 * Checks if a mouse button is currently pressed.
		 *
		 * @param button The mouse button to check.
		 * @return True if the mouse button is pressed, false otherwise.
		 */
		static bool IsMouseButtonPressed(const MouseCode button);
		/**
		 * Retrieves the current position of the mouse.
		 *
		 * @return The current position of the mouse as a 2D vector.
		 */
		static glm::vec2 GetMousePosition();
		/**
		 * @brief Retrieves the current x-coordinate of the mouse cursor.
		 *
		 * @return The x-coordinate of the mouse cursor.
		 */
		static float GetMouseX();
		/**
		 * @brief Retrieves the current y-coordinate of the mouse cursor.
		 *
		 * @return The y-coordinate of the mouse cursor.
		 */
		static float GetMouseY();
	};
    /** @} */
}