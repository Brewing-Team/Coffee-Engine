#pragma once

#include "CoffeeEngine/Core/ControllerCodes.h"
#include "CoffeeEngine/Core/KeyCodes.h"

#include <string>

namespace Coffee
{
    class InputBinding
    {
      public:
        std::string Name = "Undefined";

        KeyCode KeyPos = Key::UNKNOWN;
        KeyCode KeyNeg = Key::UNKNOWN;
        ButtonCode ButtonPos = Button::Invalid;
        ButtonCode ButtonNeg = Button::Invalid;

        AxisCode Axis = Axis::Invalid;
        bool IsAnalog = false;

        /**
         * @brief Retrieves an input value based on whether it's analog or digital.
         *
         * @param controller The input source.
         */
        float GetValue(ControllerCode controller) const;

    };
} // namespace Coffee
