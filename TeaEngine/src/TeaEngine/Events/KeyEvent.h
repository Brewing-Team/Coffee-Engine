#pragma once

#include "TeaEngine/Events/Event.h"
#include "TeaEngine/Core/KeyCodes.h"

#include <sstream>

namespace Tea {

    /**
     * @defgroup events Events
     * @{
     */

    /**
     * @brief Base class for key events.
     */
    class KeyEvent : public Event
    {
    public:
        /**
         * @brief Get the key code associated with the event.
         * @return The key code.
         */
        KeyCode GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        /**
         * @brief Constructor for KeyEvent.
         * @param keycode The key code associated with the event.
         */
        KeyEvent(const KeyCode keycode)
            : m_KeyCode(keycode) {}

        KeyCode m_KeyCode;
    };

    /**
     * @brief Event for key press.
     */
    class KeyPressedEvent : public KeyEvent
    {
    public:
        /**
         * @brief Constructor for KeyPressedEvent.
         * @param keycode The key code associated with the event.
         * @param isRepeat Whether the key press is a repeat.
         */
        KeyPressedEvent(const KeyCode keycode, bool isRepeat = false)
            : KeyEvent(keycode), m_IsRepeat(isRepeat) {}

        /**
         * @brief Check if the key press is a repeat.
         * @return True if the key press is a repeat, false otherwise.
         */
        bool IsRepeat() const { return m_IsRepeat; }

        /**
         * @brief Convert the event to a string representation.
         * @return A string representation of the event.
         */
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        bool m_IsRepeat;
    };

    /**
     * @brief Event for key release.
     */
    class KeyReleasedEvent : public KeyEvent
    {
    public:
        /**
         * @brief Constructor for KeyReleasedEvent.
         * @param keycode The key code associated with the event.
         */
        KeyReleasedEvent(const KeyCode keycode)
            : KeyEvent(keycode) {}

        /**
         * @brief Convert the event to a string representation.
         * @return A string representation of the event.
         */
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    /**
     * @brief Event for key typing.
     */
    class KeyTypedEvent : public KeyEvent
    {
    public:
        /**
         * @brief Constructor for KeyTypedEvent.
         * @param keycode The key code associated with the event.
         */
        KeyTypedEvent(const KeyCode keycode)
            : KeyEvent(keycode) {}

        /**
         * @brief Convert the event to a string representation.
         * @return A string representation of the event.
         */
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };

    /** @} */ // end of events group
}