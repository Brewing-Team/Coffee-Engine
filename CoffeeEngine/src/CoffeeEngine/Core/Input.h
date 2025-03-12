#pragma once

#include "CoffeeEngine/Core/ControllerCodes.h"
#include "CoffeeEngine/Core/KeyCodes.h"
#include "CoffeeEngine/Core/MouseCodes.h"
#include "CoffeeEngine/Input/Gamepad.h"
#include "CoffeeEngine/Events/ControllerEvent.h"
#include "CoffeeEngine/Events/KeyEvent.h"
#include "CoffeeEngine/Events/MouseEvent.h"
#include "CoffeeEngine/Input/InputBinding.h"


#include "CoffeeEngine/Events/Event.h"

#include <SDL3/SDL_gamepad.h>
#include <glm/glm.hpp>
#include <unordered_map>

namespace Coffee {
	// TODO Change for better action map layers method?
	/**
	 * @brief Current action context. Each key/button can only be in one InputAction per context layer
	 */
	enum class InputLayer : int
	{
		None = 0,
		Gameplay = BIT(1),
		Menu = BIT(2)
	};

    /**
     * @brief List of possible actions in ActionMap v0.1
     */
    enum class InputAction
    {
        // UI
    	Up,
    	Down,
    	Left,
    	Right,
        Confirm,
        Cancel,

        // Gameplay
    	MoveHorizontal,
		MoveVertical,
        Attack,
        Ability,
        Pause,

        // Action count for array creation and iteration
        ActionCount
    };

    /**
     * @defgroup core Core
     * @brief Core components of the CoffeeEngine.
     * @{
     */

	class Input
	{
	public:

        /**
         * Initializes the module
         */
        static void Init();

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
        /**
         * @brief Checks if a specific button is currently pressed on a given controller.
         *
         * @param button The button code to check.
         * @return True if the button is pressed, false otherwise.
         */
        static bool GetButtonRaw(ButtonCode button);
        /**
         * @brief Retrieves the current value of an axis on a given controller.
         *
         * @param axis The axis code to check.
         * @return The axis value, usually between -1 and 1. Returns 0 if the controller is invalid.
         */
        static float GetAxisRaw(AxisCode axis);

		static InputLayer CurrentInputContext;

        static void OnEvent(Event& e);

	private:
        /**
	     * @brief Handles controller connection events
	     * @param cEvent The event data to process
	     */
        static void OnAddController(const ControllerAddEvent* cEvent);
	    /**
         * @brief Handles controller disconnection events
         * @param cEvent The event data to process
         */
	    static void OnRemoveController(const ControllerRemoveEvent* cEvent);
	    /**
        * @brief Handles button press events from controllers.
        *
        * @param e The button press event to process.
        */
        static void OnButtonPressed(const ButtonPressEvent& e);
	    /**
         * @brief Handles button release events from controllers.
         *
         * @param e The button release event to process.
         */
        static void OnButtonReleased(const ButtonReleaseEvent& e);
	    /**
         * @brief Handles axis movement events from controllers.
         *
         * @param e The axis move event to process.
         */
        static void OnAxisMoved(const AxisMoveEvent& e);
	    /**
         * @brief Handles key press events from the keyboard.
         *
         * @param event The key pressed event to process.
         */
	    static void OnKeyPressed(const KeyPressedEvent& event);
	    /**
         * @brief Handles key release events from the keyboard.
         *
         * @param event The key released event to process.
         */
	    static void OnKeyReleased(const KeyReleasedEvent& event);
	    /**
         * @brief Handles mouse button press events.
         *
         * @param event The mouse button pressed event to process.
         */
	    static void OnMouseButtonPressed(const MouseButtonPressedEvent& event);
	    /**
         * @brief Handles mouse button release events.
         *
         * @param event The mouse button released event to process.
         */
	    static void OnMouseButtonReleased(const MouseButtonReleasedEvent& event);
	    /**
         * @brief Handles mouse movement events.
         *
         * @param event The mouse moved event to process.
         */
	    static void OnMouseMoved(const MouseMovedEvent& event);

        static std::vector<InputBinding> m_bindings;

	    static std::vector<Ref<Gamepad>> m_gamepads;
	    static std::unordered_map<ButtonCode, char> m_buttonStates;
	    static std::unordered_map<AxisCode, float> m_axisStates;
	    static std::unordered_map<KeyCode, bool> m_keyStates;

	};
    /** @} */
}