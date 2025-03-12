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

        KeyCode KeyPos = Key::Unknown;
        KeyCode KeyNeg = Key::Unknown;
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
