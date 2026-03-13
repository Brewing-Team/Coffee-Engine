#pragma once

#include "CoffeeEngine/Core/ControllerCodes.h"
#include "CoffeeEngine/Core/KeyCodes.h"
#include "CoffeeEngine/Core/MouseCodes.h"
#include "CoffeeEngine/Events/ControllerEvent.h"
#include "CoffeeEngine/Events/KeyEvent.h"
#include "CoffeeEngine/Events/MouseEvent.h"
#include "CoffeeEngine/Input/Gamepad.h"
#include "CoffeeEngine/Input/InputBinding.h"
#include "Timer.h"

#include "CoffeeEngine/Events/Event.h"

#include <SDL3/SDL_gamepad.h>
#include <glm/glm.hpp>
#include <unordered_map>

namespace Coffee {

    class Window;

    using InputAction = uint16_t;
    /**
     * @brief List of possible actions in ActionMap v0.1
     */
    namespace ActionsEnum
    {
        enum : InputAction
        {
            // UI
            UiMoveHorizontal,
            UiMoveVertical,
            Confirm,
            Cancel,

            // Gameplay
            MoveHorizontal,
            MoveVertical,
            AimHorizontal,
            AimVertical,
            Shoot,
            Melee,
            Interact,
            Dash,
            Cover,
            Skill1,
            Skill2,
            Skill3,
            Injector,
            Grenade,
            Map,
            Pause,

            // Action count for array creation and iteration
            ActionCount
        };
    } // namespace ActionsEnum

    /**
     * @brief Possible states for rebinding inputs
     */
    enum class RebindState
    {
        None,
        PosButton,
        NegButton,
        PosKey,
        NegKey,
        Axis
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
        Input(Window* window);

/*         void Save();

        void Load(); */

        /**
         * Checks if a specific key is currently being pressed.
         *
         * @param key The key code of the key to check.
         * @return True if the key is currently being pressed, false otherwise.
         */
        bool IsKeyPressed(const KeyCode key);

        /**
         * Checks if a mouse button is currently pressed.
         *
         * @param button The mouse button to check.
         * @return True if the mouse button is pressed, false otherwise.
         */
        bool IsMouseButtonPressed(const MouseCode button);

        /**
            * Sets the mouse cursor to be grabbed or ungrabbed.
            * When grabbed, the mouse cursor is confined to the window and hidden.
            * When ungrabbed, the mouse cursor is free to move outside the window.
            * @param grabbed True to grab the mouse cursor, false to ungrab it.
            */
        void SetMouseGrabbed(bool grabbed);

        /**
         * Retrieves the current position of the mouse.
         *
         * @return The current position of the mouse as a 2D vector.
         */
        const glm::vec2& GetMousePosition();
        /**
         * @brief Retrieves the current x-coordinate of the mouse cursor.
         *
         * @return The x-coordinate of the mouse cursor.
         */
        const float GetMouseX();
        /**
         * @brief Retrieves the current y-coordinate of the mouse cursor.
         *
         * @return The y-coordinate of the mouse cursor.
         */

        const float GetMouseY();

        glm::vec2 GetMouseDelta();
        /**
         * @brief Checks if a specific button is currently pressed on a given controller.
         *
         * @param button The button code to check.
         * @return True if the button is pressed, false otherwise.
         */
        bool GetButtonRaw(ButtonCode button);
        /**
         * @brief Retrieves the current value of an axis on a given controller.
         *
         * @param axis The axis code to check.
         * @return The axis value, usually between -1 and 1. Returns 0 if the controller is invalid.
         */
        float GetAxisRaw(AxisCode axis);

        /**
         * Gets the InputBinding object for the given action
         * @param actionName The action to retrieve an InputBinding for
         * @return The InputBinding containing the bounds keys, buttons and axis for the provided action
         */
        InputBinding& GetBinding(const std::string& actionName);

        std::unordered_map<std::string, InputBinding>& GetAllBindings();

        /**
         *
         * @param lowFreqPower Strength of the left (low frequency) motor
         * @param highFreqPower Strength of the right (high frequency) motor
         * @param duration Vibration duration
         */
        void SendRumble(uint16_t lowFreqPower, uint16_t highFreqPower, uint32_t duration);

        const char* GetKeyLabel(KeyCode key);
        const char* GetMouseButtonLabel(MouseCode button);
        const char* GetButtonLabel(ButtonCode button);
        const char* GetAxisLabel(AxisCode axis);

        void StartRebindMode(std::string actionName, RebindState type);
        void ResetRebindState();

        void OnEvent(Event& e);

	    long OnFrameUpdate();

      private:

        void GenerateDefaultMappingFile();

        /**
	     * @brief Handles controller connection events
	     * @param cEvent The event data to process
	     */
        void OnAddController(const ControllerAddEvent* cEvent);
	    /**
         * @brief Handles controller disconnection events
         * @param cEvent The event data to process
         */
	    void OnRemoveController(const ControllerRemoveEvent* cEvent);
	    /**
        * @brief Handles button press events from controllers.
        *
        * @param e The button press event to process.
        */
        void OnButtonPressed(const ButtonPressEvent& e);
	    /**
         * @brief Handles button release events from controllers.
         *
         * @param e The button release event to process.
         */
        void OnButtonReleased(const ButtonReleaseEvent& e);
	    /**
         * @brief Handles axis movement events from controllers.
         *
         * @param e The axis move event to process.
         */
        void OnAxisMoved(const AxisMoveEvent& e);
	    /**
         * @brief Handles key press events from the keyboard.
         *
         * @param event The key pressed event to process.
         */
	    void OnKeyPressed(const KeyPressedEvent& event);
	    /**
         * @brief Handles key release events from the keyboard.
         *
         * @param event The key released event to process.
         */
	    void OnKeyReleased(const KeyReleasedEvent& event);
	    /**
         * @brief Handles mouse button press events.
         *
         * @param event The mouse button pressed event to process.
         */
	    void OnMouseButtonPressed(const MouseButtonPressedEvent& event);
	    /**
         * @brief Handles mouse button release events.
         *
         * @param event The mouse button released event to process.
         */
	    void OnMouseButtonReleased(const MouseButtonReleasedEvent& event);
	    /**
         * @brief Handles mouse movement events.
         *
         * @param event The mouse moved event to process.
         */
	    void OnMouseMoved(const MouseMovedEvent& event);
    
    private:
        std::unordered_map<std::string, InputBinding> m_BindingsMap;

	    std::vector<Ref<Gamepad>> m_Gamepads;
	    std::unordered_map<ButtonCode, uint8_t> m_ButtonStates = {{Button::Invalid, 0}};;
	    std::unordered_map<AxisCode, float> m_AxisStates = {{Axis::Invalid, 0.0f}};
	    std::unordered_map<KeyCode, bool> m_KeyStates = {{Key::Unknown, false}};
        std::unordered_map<MouseCode, bool> m_MouseStates;
        std::unordered_map<AxisCode, float> m_AxisDeadzones;
        glm::vec2 m_MousePosition = glm::vec2(0.0f); // Position relative to window

        // Rebind mode
        Timer m_RebindTimer;
        RebindState m_RebindState = RebindState::None;
	    std::string m_RebindActionName;

	    // Input timestamps to prevent recalculating values multiple times in the same frame
	    long m_Timestamp;

	    friend class InputBinding; // Allow direct access to private variables from InputBinding

        Window* m_Window;
    };
    /** @} */
} // namespace Coffee