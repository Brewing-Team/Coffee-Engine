#pragma once

#include "Base.h"
#include "CoffeeEngine/Audio/Audio.h"
#include "CoffeeEngine/Core/EngineContext.h"
#include "CoffeeEngine/Core/Input.h"
#include "CoffeeEngine/Renderer/Renderer.h"
#include "CoffeeEngine/Renderer/RendererAPI.h"
#include "Window.h"
#include "LayerStack.h"
#include "CoffeeEngine/Events/ApplicationEvent.h"
#include "CoffeeEngine/ImGui/ImGuiLayer.h"

#include <functional>

namespace Coffee
{
	class Application;

    /**
     * @defgroup core Core
     * @brief Core components of the CoffeeEngine.
     * @{
     */

    /**
     * @brief The Engine class is responsible for managing the main engine loop,
     * handling events, and managing layers and overlays.
     */
    class Engine
    {
      public:
        Renderer renderer; ///< The renderer used by the engine.
        RendererAPI rendererAPI; ///< The renderer API used by the engine.
        Input input; ///< The input system used by the engine.
        Audio audio; ///< The audio system used by the engine.
        
      public:
        using EventCallbackFn = std::function<void(Event&)>; ///< Type definition for event callback function.
        /**
         * @brief Constructs the Engine object.
         */
        Engine();

        /**
         * @brief Destroys the Engine object.
         */
        virtual ~Engine();

        /**
         * @brief Starts the main Engine loop.
         */
        void Run(Application& app);

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

        EngineContext GetContext() { return { &renderer, &rendererAPI, &input, &resources, &jobs };

        /**
         * @brief Gets the main engine window.
         * @return A reference to the main engine window.
         */
        Window& GetWindow() { return *m_Window; }

        /**
         * @brief Sets the event callback function.
         * @param callback The event callback function.
         */
        void SetEventCallback(const EventCallbackFn& callback) { m_EventCallback = callback; }

        /**
         * @brief Closes the Engine.
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

        Scope<Window> m_Window; ///< The main engine window.
        ImGuiLayer* m_ImGuiLayer; ///< The ImGui layer.
        bool m_Running = true; ///< Indicates whether the engine is running.
        LayerStack m_LayerStack; ///< The stack of layers.
        double m_LastFrameTime = 0.0f; ///< The time of the last frame.
        EventCallbackFn m_EventCallback; ///< The event callback function.
    };

    /** @} */
} // namespace Coffee