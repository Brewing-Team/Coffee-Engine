#pragma once

#include "TeaEngine/Events/Event.h"

#include <string>
#include <sstream>

namespace Tea {

    /**
     * @defgroup events Events
     * @{
     */

    /**
     * @brief Event for window resize.
     */
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
            : m_Width(width), m_Height(height) {}

        /**
         * @brief Get the width of the window.
         * @return The width of the window.
         */
        unsigned int GetWidth() const { return m_Width; }

        /**
         * @brief Get the height of the window.
         * @return The height of the window.
         */
        unsigned int GetHeight() const { return m_Height; }

        /**
         * @brief Convert the event to a string representation.
         * @return A string representation of the event.
         */
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    private:
        unsigned int m_Width, m_Height;
    };

    /**
     * @brief Event for window close.
     */
    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    /**
     * @brief Event for application tick.
     */
    class AppTickEvent : public Event
    {
    public:
        AppTickEvent() = default;

        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    /**
     * @brief Event for application update.
     */
    class AppUpdateEvent : public Event
    {
    public:
        AppUpdateEvent() = default;

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    /**
     * @brief Event for application render.
     */
    class AppRenderEvent : public Event
    {
    public:
        AppRenderEvent() = default;

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    /** @} */ // end of events group
}