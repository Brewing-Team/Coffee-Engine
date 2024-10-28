#pragma once

#include "CoffeeEngine/Events/Event.h"

#include <cstdint>
#include <string>
#include <sstream>

namespace Coffee {

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
     * @brief Event for Dropping a file.
     */
    class FileDropEvent : public Event
    {
    public:
        FileDropEvent(uint64_t timestamp, uint32_t windowID, float x, float y, const std::string& source, const std::string& file)
            : m_Timestamp(timestamp), m_WindowID(windowID), m_X(x), m_Y(y), m_Source(source), m_File(file) {}
        
        /**
            * @brief Get the timestamp of the event.
            * @return The timestamp of the event.
            */
        uint64_t GetTimestamp() const { return m_Timestamp; }

        /**
            * @brief Get the window ID of the event.
            * @return The window ID of the event.
            */
        uint32_t GetWindowID() const { return m_WindowID; }

        /**
            * @brief Get the x position of the event.
            * @return The x position of the event.
            */
        float GetX() const { return m_X; }

        /**
            * @brief Get the y position of the event.
            * @return The y position of the event.
            */
        float GetY() const { return m_Y; }

        /**
            * @brief Get the source of the event.
            * @return The source of the event.
            */
        const std::string& GetSource() const { return m_Source; }

        /**
            * @brief Get the file of the event.
            * @return The file of the event.
            */
        const std::string& GetFile() const { return m_File; }

        /**
            * @brief Convert the event to a string representation.
            * @return A string representation of the event.
            */
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "FileDropEvent: " << m_Timestamp << ", " << m_WindowID << ", " << m_X << ", " << m_Y << ", " << m_Source << ", " << m_File;
            return ss.str();
        }

        EVENT_CLASS_TYPE(FileDrop)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    private:
        uint64_t m_Timestamp;
        uint32_t m_WindowID;
        float m_X;
        float m_Y;
        std::string m_Source;
        std::string m_File;
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