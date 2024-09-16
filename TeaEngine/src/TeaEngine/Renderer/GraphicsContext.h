#pragma once

#include "TeaEngine/Core/Base.h"

class GLFWwindow;

namespace Tea {

    /**
     * @defgroup renderer Renderer
     * @brief Renderer components of the TeaEngine.
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
         * @param windowHandle The handle to the GLFW window.
         */
        GraphicsContext(GLFWwindow* windowHandle);

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
         * @param window The handle to the GLFW window.
         * @return A scope pointer to the created graphics context.
         */
        static Scope<GraphicsContext> Create(GLFWwindow* window);
    private:
        GLFWwindow* m_WindowHandle; ///< The handle to the GLFW window.
    };

    /** @} */
}