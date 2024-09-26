#pragma once

#include "CoffeeEngine/Events/Event.h"
#include "CoffeeEngine/Core/MouseCodes.h"

#include <sstream>

namespace Coffee {

    /**
     * @defgroup events Events
     * @{
     */

    /**
     * @brief Event for mouse movement.
     * @ingroup events
     */
    class MouseMovedEvent : public Event
    {
    public:
        /**
         * @brief Constructor for MouseMovedEvent.
         * @param x The x-coordinate of the mouse.
         * @param y The y-coordinate of the mouse.
         */
        MouseMovedEvent(const float x, const float y)
            : m_MouseX(x), m_MouseY(y) {}

        /**
         * @brief Get the x-coordinate of the mouse.
         * @return The x-coordinate.
         */
        float GetX() const { return m_MouseX; }

        /**
         * @brief Get the y-coordinate of the mouse.
         * @return The y-coordinate.
         */
        float GetY() const { return m_MouseY; }

        /**
         * @brief Convert the event to a string representation.
         * @return A string representation of the event.
         */
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_MouseX, m_MouseY;
    };

    /**
     * @brief Event for mouse scrolling.
     * @ingroup events
     */
    class MouseScrolledEvent : public Event
    {
    public:
        /**
         * @brief Constructor for MouseScrolledEvent.
         * @param xOffset The scroll offset along the x-axis.
         * @param yOffset The scroll offset along the y-axis.
         */
        MouseScrolledEvent(const float xOffset, const float yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset) {}

        /**
         * @brief Get the scroll offset along the x-axis.
         * @return The x-axis scroll offset.
         */
        float GetXOffset() const { return m_XOffset; }

        /**
         * @brief Get the scroll offset along the y-axis.
         * @return The y-axis scroll offset.
         */
        float GetYOffset() const { return m_YOffset; }

        /**
         * @brief Convert the event to a string representation.
         * @return A string representation of the event.
         */
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_XOffset, m_YOffset;
    };

    /**
     * @brief Base class for mouse button events.
     * @ingroup events
     */
    class MouseButtonEvent : public Event
    {
    public:
        /**
         * @brief Get the mouse button associated with the event.
         * @return The mouse button.
         */
        MouseCode GetMouseButton() const { return m_Button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
    protected:
        /**
         * @brief Constructor for MouseButtonEvent.
         * @param button The mouse button associated with the event.
         */
        MouseButtonEvent(const MouseCode button)
            : m_Button(button) {}

        MouseCode m_Button;
    };

    /**
     * @brief Event for mouse button press.
     * @ingroup events
     */
    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        /**
         * @brief Constructor for MouseButtonPressedEvent.
         * @param button The mouse button associated with the event.
         */
        MouseButtonPressedEvent(const MouseCode button)
            : MouseButtonEvent(button) {}

        /**
         * @brief Convert the event to a string representation.
         * @return A string representation of the event.
         */
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    /**
     * @brief Event for mouse button release.
     * @ingroup events
     */
    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        /**
         * @brief Constructor for MouseButtonReleasedEvent.
         * @param button The mouse button associated with the event.
         */
        MouseButtonReleasedEvent(const MouseCode button)
            : MouseButtonEvent(button) {}

        /**
         * @brief Convert the event to a string representation.
         * @return A string representation of the event.
         */
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

    /** @} */ // end of events group
}