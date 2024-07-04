#pragma once

#include "TeaEngine/Core/Base.h"

class GLFWwindow;

namespace Tea {

    class GraphicsContext
    {
    public:
        GraphicsContext(GLFWwindow* windowHandle);
        virtual ~GraphicsContext();

        void Init();
        void SwapBuffers();

        static Scope<GraphicsContext> Create(GLFWwindow* window);
    private:
        GLFWwindow* m_WindowHandle;
    };

}