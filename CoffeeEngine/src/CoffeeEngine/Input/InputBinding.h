#pragma once

#include "CoffeeEngine/Core/ControllerCodes.h"
#include "CoffeeEngine/Core/KeyCodes.h"

#include <string>

namespace Coffee
{
    using ButtonState = uint8_t;
    namespace ButtonStates
    {
        enum : ButtonState
        {
            IDLE,
            UP,
            DOWN,
            REPEAT
        };
    } // namespace ButtonStates

    /**
     * @defgroup core Input
     * @brief Data structure containing the bound buttons, keys and axis of an input action
     * @{
     */
    class InputBinding
    {
      public:
        // This is only used for easier identification within serialized files,debug mode or config UI (if implemented)
        std::string Name = "Undefined";

        KeyCode KeyPos = Key::Unknown;
        KeyCode KeyNeg = Key::Unknown;
        ButtonCode ButtonPos = Button::Invalid;
        ButtonCode ButtonNeg = Button::Invalid;

        AxisCode Axis = Axis::Invalid;
        float Deadzone = 0.2f;

        /**
         * @brief Retrieves an input value from the action as a controller axis. It doesn't need to be an actual axis to
         * retrieve a value
         * @param digital whether the output should be truncated or not
         * @return A value between -1 and 1
         */
        float AsAxis(bool digital) const;

        /**
         * @brief Returns the value of the action as a boolean. For axes it returns true if axisVal != 0
         * @return True if any of the bound keys or buttons are pressed, or if the axis is not at 0
         */
        bool AsBool();

        /**
         * @brief Retrieves an input value from the action as a controller button. Can be used on axis and will return
         * DOWN or REPEAT if its value surpasses the defined deadzone
         * @return The current state of the action as a button
         */
        ButtonState AsButton();

    private:
        ButtonState m_State = ButtonStates::IDLE;

    };
    /** @} */
} // namespace Coffee
