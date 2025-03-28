#pragma once

#include "Base.h"
#include "Window.h"
#include "LayerStack.h"
#include "CoffeeEngine/Events/ApplicationEvent.h"
#include "CoffeeEngine/ImGui/ImGuiLayer.h"

namespace Coffee
{
    /**
     * @defgroup core Core
     * @brief Core components of the CoffeeEngine.
     * @{
     */

    /**
     * @brief The Application class is responsible for managing the main application loop,
     * handling events, and managing layers and overlays.
     */
    class Application
    {
      public:
        using EventCallbackFn = std::function<void(Event&)>; ///< Type definition for event callback function.
        /**
         * @brief Constructs the Application object.
         */
        Application();

        /**
         * @brief Destroys the Application object.
         */
        virtual ~Application();

        /**
         * @brief Starts the main application loop.
         */
        void Run();

        /**
         * @brief Handles incoming events.
         * @param e The event to handle.
         */
        void OnEvent(Event& e);

        /**
         * @brief Pushes a layer onto the layer stack.
         * @param layer The layer to push.
         */
        void PushLayer(Layer* layer);

        /**
         * @brief Pushes an overlay onto the layer stack.
         * @param layer The overlay to push.
         */
        void PushOverlay(Layer* layer);

        /**
         * @brief Gets the main application window.
         * @return A reference to the main application window.
         */
        Window& GetWindow() { return *m_Window; }

        /**
         * @brief Sets the event callback function.
         * @param callback The event callback function.
         */
        void SetEventCallback(const EventCallbackFn& callback) { m_EventCallback = callback; }

        /**
         * @brief Closes the application.
         */
        void Close();

        /**
         * @brief Gets the ImGui layer.
         * @return A pointer to the ImGui layer.
         */
        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        // Temporary until we have a proper way to get the FPS and FrameTime
        float GetFrameTime() const { return m_LastFrameTime * 1000.0f; }
        float GetFPS() const { return 1.0f / m_LastFrameTime; }

        /**
         * @brief Gets the singleton instance of the Application.
         * @return A reference to the singleton instance.
         */
        static Application& Get() { return *s_Instance; }

      private:

        /**
         * @brief Polls and processes events.
         * 
         * This function retrieves and handles events such as input from the keyboard,
         * mouse, window, and other devices.
         */
        void ProcessEvents();

        /**
         * @brief Handles the window close event.
         * @param e The window close event.
         * @return True if the event was handled, false otherwise.
         */
        bool OnWindowClose(WindowCloseEvent& e);

      private:

        Scope<Window> m_Window; ///< The main application window.
        ImGuiLayer* m_ImGuiLayer; ///< The ImGui layer.
        bool m_Running = true; ///< Indicates whether the application is running.
        LayerStack m_LayerStack; ///< The stack of layers.
        double m_LastFrameTime = 0.0f; ///< The time of the last frame.
        EventCallbackFn m_EventCallback; ///< The event callback function.

      private:
        static Application* s_Instance; ///< The singleton instance of the Application.
    };

    /**
     * @brief Creates the application. To be defined in the client.
     * @return A pointer to the created application.
     */
    Application* CreateApplication();

    /** @} */
} // namespace Coffee