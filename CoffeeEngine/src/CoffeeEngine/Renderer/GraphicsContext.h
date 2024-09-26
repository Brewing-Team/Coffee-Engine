#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "SDL3/SDL_video.h"

namespace Coffee {

    /**
     * @defgroup renderer Renderer
     * @brief Renderer components of the CoffeeEngine.
     * @{
     */

    /**
     * @brief Class representing the graphics context.
     */
    class GraphicsContext
    {
    public:
        /**
         * @brief Constructs a GraphicsContext with the specified window handle.
         * @param windowHandle The handle to the SDL window.
         */
        GraphicsContext(SDL_Window* windowHandle);

        /**
         * @brief Virtual destructor for the GraphicsContext class.
         */
        virtual ~GraphicsContext();

        /**
         * @brief Initializes the graphics context.
         */
        void Init();

        /**
         * @brief Swaps the front and back buffers.
         */
        void SwapBuffers();

        /**
         * @brief Creates a graphics context for the specified window.
         * @param window The handle to the SDL window.
         * @return A scope pointer to the created graphics context.
         */
        static Scope<GraphicsContext> Create(SDL_Window* window);
    private:
        SDL_Window* m_WindowHandle; ///< The handle to the SDL window.
    };

    /** @} */
}