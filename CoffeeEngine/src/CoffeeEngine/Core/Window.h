#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Events/Event.h"
#include "CoffeeEngine/Renderer/GraphicsContext.h"

#include <cstdint>
#include <functional>
#include <sstream>

struct GLFWwindow;

namespace Coffee {

    /**
     * @defgroup core Core
     * @brief Core components of the CoffeeEngine.
     * @{
     */

    /**
     * @brief Structure to hold window properties such as title, width, and height.
     */
    struct WindowProps
    {
        std::string Title; ///< The title of the window.
        uint32_t Width; ///< The width of the window.
        uint32_t Height; ///< The height of the window.

        /**
         * @brief Constructs WindowProps with default or specified values.
         * @param title The title of the window.
         * @param width The width of the window.
         * @param height The height of the window.
         */
        WindowProps(const std::string& title = "Coffee Engine",
                    uint32_t width = 1600,
                    uint32_t height = 900)
            : Title(title), Width(width), Height(height)
        {
        }
    };

    /**
     * @brief The Window class is responsible for managing the window and its properties.
     */
    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>; ///< Type definition for event callback function.

        /**
         * @brief Constructs a Window object with the specified properties.
         * @param props The properties of the window.
         */
        Window(const WindowProps& props);

        /**
         * @brief Destroys the Window object.
         */
        virtual ~Window();

        /**
         * @brief Updates the window.
         */
        void OnUpdate();

        /**
         * @brief Gets the width of the window.
         * @return The width of the window.
         */
        unsigned int GetWidth() const { return m_Data.Width; }

        /**
         * @brief Gets the height of the window.
         * @return The height of the window.
         */
        unsigned int GetHeight() const { return m_Data.Height; }

        /**
         * @brief Sets the event callback function.
         * @param callback The event callback function.
         */
        void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

        /**
         * @brief Enables or disables VSync.
         * @param enabled True to enable VSync, false to disable.
         */
        void SetVSync(bool enabled);

        /**
         * @brief Checks if VSync is enabled.
         * @return True if VSync is enabled, false otherwise.
         */
        bool IsVSync() const;

        /**
         * @brief Sets the title of the window.
         * @param title The new title of the window.
         */
        void SetTitle(const std::string& title);

        /**
         * @brief Gets the title of the window.
         * @return The title of the window.
         */
        const std::string& GetTitle() const { return m_Data.Title; }

        /**
         * @brief Sets the icon of the window.
         * @param path The path to the icon file.
         */
        void SetIcon(const std::string& path);

        /**
         * @brief Gets the native window handle.
         * @return A pointer to the native window.
         */
        virtual void* GetNativeWindow() const { return m_Window; }

        /**
         * @brief Creates a window with the specified properties.
         * @param props The properties of the window.
         * @return A scoped pointer to the created window.
         */
        static Scope<Window> Create(const WindowProps& props = WindowProps()) { return CreateScope<Window>(props); }

    private:
        /**
         * @brief Initializes the window with the specified properties.
         * @param props The properties of the window.
         */
        virtual void Init(const WindowProps& props);

        /**
         * @brief Shuts down the window.
         */
        virtual void Shutdown();

    private:
        GLFWwindow* m_Window; ///< Pointer to the GLFW window.
        Scope<GraphicsContext> m_Context; ///< Scoped pointer to the graphics context.

        /**
         * @brief Structure to hold window data such as title, width, height, and VSync status.
         */
        struct WindowData
        {
            std::string Title; ///< The title of the window.
            unsigned int Width, Height; ///< The width and height of the window.
            bool VSync; ///< Indicates whether VSync is enabled.

            EventCallbackFn EventCallback; ///< The event callback function.
        };

        WindowData m_Data; ///< The window data.
    };

    /** @} */
} // namespace Coffee