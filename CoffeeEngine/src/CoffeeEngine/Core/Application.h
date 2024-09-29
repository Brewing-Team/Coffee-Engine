#pragma once

#include "Base.h"
#include "SDL3/SDL_stdinc.h"
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
         * @brief Closes the application.
         */
        void Close();

        /**
         * @brief Gets the ImGui layer.
         * @return A pointer to the ImGui layer.
         */
        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        /**
         * @brief Gets the singleton instance of the Application.
         * @return A reference to the singleton instance.
         */
        static Application& Get() { return *s_Instance; }

      private:
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
        Uint64 m_LastFrameTime = 0.0f; ///< The time of the last frame.

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