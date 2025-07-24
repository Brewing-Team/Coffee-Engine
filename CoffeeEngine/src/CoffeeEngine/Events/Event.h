#pragma once

#include "CoffeeEngine/Core/Base.h"

#include <fmt/base.h>
#include <fmt/format.h>
#include <string>

namespace Coffee {

    /**
     * @defgroup events Events
     * @{
     */

    /**
     * @brief Enum representing different types of events.
     */
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, FileDrop,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
        ButtonPressed, ButtonReleased, AxisMoved,
        ControllerConnected, ControllerDisconnected,
    };

    /**
     * @brief Enum representing different event categories.
     */
    enum EventCategory
    {
        None = 0,
        EventCategoryApplication    = BIT(0),
        EventCategoryInput          = BIT(1),
        EventCategoryKeyboard       = BIT(2),
        EventCategoryMouse          = BIT(3),
        EventCategoryMouseButton    = BIT(4),
        EventCategoryControllerAxis = BIT(5),
        EventCategoryControllerButton = BIT(6),
    };

    /**
     * @brief Macro to define the type of an event.
     */
    #define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                                virtual EventType GetEventType() const override { return GetStaticType(); }\
                                virtual const char* GetName() const override { return #type; }

    /**
     * @brief Macro to define the category of an event.
     */
    #define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    /**
     * @brief Base class for all events.
     */
    class Event
    {
    public:
        virtual ~Event() = default;

        bool Handled = false;

        /**
         * @brief Get the type of the event.
         * @return The event type.
         */
        virtual EventType GetEventType() const = 0;

        /**
         * @brief Get the name of the event.
         * @return The event name.
         */
        virtual const char* GetName() const = 0;

        /**
         * @brief Get the category flags of the event.
         * @return The category flags.
         */
        virtual int GetCategoryFlags() const = 0;

        /**
         * @brief Convert the event to a string representation.
         * @return A string representation of the event.
         */
        virtual std::string ToString() const { return GetName(); }

        /**
         * @brief Check if the event is in a specific category.
         * @param category The category to check.
         * @return True if the event is in the category, false otherwise.
         */
        bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }
    };

    /**
     * @brief Event dispatcher to handle events.
     */
    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
            : m_Event(event)
        {
        }
        
        /**
         * @brief Dispatches the event if it matches the type.
         * @tparam T The event type.
         * @tparam F The function type.
         * @param func The function to call if the event matches the type.
         * @return True if the event was dispatched, false otherwise.
         */
        template<typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.Handled |= func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }
    private:
        Event& m_Event;
    };

    /**
     * @brief Overload of the << operator for Event.
     * @param os The output stream.
     * @param e The event.
     * @return The output stream.
     */
    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }

    /** @} */ // end of events group
}

//Fix for printing an event with fmt (thanks @ilex8015!)
template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of<Coffee::Event,T>::value, char>> : fmt::formatter<std::string>
{
	auto format(const T& event, fmt::format_context& ctx)
	{
		return fmt::format_to(ctx.out(), "{}", event.ToString());
	}
};

template <typename... T>
std::string StringFromArgs(fmt::format_string<T...>fmt, T&&... args)
{
	return fmt::format(fmt, std::forward<T>(args)...);
}
