#pragma once

#include "Event.h"
#include "CoffeeEngine/Core/ControllerCodes.h"

namespace Coffee
{

    /**
     * Axis event
     * Ignore the controller code to accept axis input from all controllers
     */
    class AxisMoveEvent : public Event
    {
    public:
        AxisMoveEvent(ControllerCode id, AxisCode code, float value) : Controller(id), Axis(code), Value(value)
        {}

        ControllerCode Controller;
        AxisCode Axis;
        float Value;

        EVENT_CLASS_TYPE(AxisMoved)
        EVENT_CLASS_CATEGORY(EventCategoryControllerAxis | EventCategoryInput)
    };

    /**
     * Button press event
     * Ignore the controller code to accept axis input from all controllers
     */
    class ButtonPressEvent : public Event
    {
    public:
        ButtonPressEvent(ControllerCode id, ButtonCode code) : Controller(id), Button(code)
        {}
        ControllerCode Controller;
        ButtonCode Button;

        EVENT_CLASS_TYPE(ButtonPressed)
        EVENT_CLASS_CATEGORY(EventCategoryControllerButton | EventCategoryInput)
    };

    /**
     * Button release event
     * Ignore the controller code to accept axis input from all controllers
     */
    class ButtonReleaseEvent : public Event
    {
    public:
        ButtonReleaseEvent(ControllerCode id, ButtonCode code) : Controller(id), Button(code)
        {}
        ControllerCode Controller;
        ButtonCode Button;

        EVENT_CLASS_TYPE(ButtonReleased)
        EVENT_CLASS_CATEGORY(EventCategoryControllerButton | EventCategoryInput)
    };

    /**
     * Controller detected event
     * Dispatched when a new controller is detected or the mapping for an existing controller is changed
     */
    class ControllerAddEvent : public Event
    {
    public:
        ControllerAddEvent(ControllerCode id) : Controller(id)
        {}
        ControllerCode Controller;

        EVENT_CLASS_TYPE(ControllerConnected)
        EVENT_CLASS_CATEGORY(EventCategoryInput)
    };

    /**
     * Controller disconnection event
     * Dispatched when a controller is disconnected
     */
    class ControllerRemoveEvent : public Event
    {
    public:
        ControllerRemoveEvent(ControllerCode id) :Controller(id)
        {}
        ControllerCode Controller;

        EVENT_CLASS_TYPE(ControllerDisconnected)
        EVENT_CLASS_CATEGORY(EventCategoryInput)
    };

} // Coffee

