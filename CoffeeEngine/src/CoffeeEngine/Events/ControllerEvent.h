#pragma once

#include "Event.h"
#include "CoffeeEngine/Core/ControllerCodes.h"

namespace Coffee
{

    class AxisMoveEvent : public Event
    {
        public:
            AxisMoveEvent(AxisCode code, float value) : Axis(code), Value(value)
            {}

        AxisCode Axis;
        float Value;

        EVENT_CLASS_TYPE(AxisMoved)
        EVENT_CLASS_CATEGORY(EventCategoryControllerAxis | EventCategoryInput)
    };

    class ButtonPressEvent : public Event
    {
        public:
            ButtonPressEvent(ButtonCode code) : Button(code)
            {}
        ButtonCode Button;

        EVENT_CLASS_TYPE(ButtonPressed)
        EVENT_CLASS_CATEGORY(EventCategoryControllerButton | EventCategoryInput)
    };

    class ButtonReleaseEvent : public Event
    {
        public:
            ButtonReleaseEvent(ButtonCode code) : Button(code)
            {}
        ButtonCode Button;

        EVENT_CLASS_TYPE(ButtonReleased)
        EVENT_CLASS_CATEGORY(EventCategoryControllerButton | EventCategoryInput)
    };

} // Coffee

