#pragma once

#include "CoffeeEngine/Core/ControllerCodes.h"
#include "CoffeeEngine/Core/KeyCodes.h"
#include "CoffeeEngine/Core/MouseCodes.h"
#include "CoffeeEngine/Input/Gamepad.h"
#include "CoffeeEngine/Events/ControllerEvent.h"

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

    /** Struct containing input details for a given action.
     *
     */
    class InputBinding
    {
		std::string Name = "Undefined";

        // Digital
        KeyCode KeyPos = Key::UNKNOWN; // Positive keyboard key
        KeyCode KeyNeg = Key::UNKNOWN; // Negative keyboard key
        ButtonCode ButtonPos = Button::Invalid; // Positive gamepad button
        ButtonCode ButtonNeg = Button::Invalid; // Negative gamepad button

        // Analog
        KeyCode Axis = Axis::Invalid; // Defined axis for analog control
        float AxisVal = 0; // Axis value

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
         * @param controller The controller code (default is 0, meaning invalid controller).
         * @return True if the button is pressed, false otherwise.
         */
        static bool GetButton(ButtonCode button, ControllerCode controller);
        /**
         * @brief Retrieves the current value of an axis on a given controller.
         *
         * @param axis The axis code to check.
         * @param controller The controller code (default is 0, meaning invalid controller).
         * @return The axis value, usually between -1 and 1. Returns 0 if the controller is invalid.
         */
        static float GetAxis(AxisCode axis, ControllerCode controller);

		static InputLayer CurrentInputContext;

        static void OnEvent(Event& e);

	    private:

	    static void OnAddController(ControllerAddEvent* cEvent);
	    static void OnRemoveController(ControllerRemoveEvent* event);
        static void OnButtonPressed(ButtonPressEvent& e);
        static void OnButtonReleased(ButtonReleaseEvent& e);
        static void OnAxisMoved(AxisMoveEvent& e);

        static std::vector<InputBinding> m_bindings;

	    static std::vector<Ref<Gamepad>> m_gamepads;
	    static std::unordered_map<ControllerCode, std::unordered_map<ButtonCode, bool>> m_buttonStates;
	    static std::unordered_map<ControllerCode, std::unordered_map<AxisCode, float>> m_axisStates;
	    static std::unordered_map<KeyCode, bool> m_keyStates;

	};
    /** @} */
}