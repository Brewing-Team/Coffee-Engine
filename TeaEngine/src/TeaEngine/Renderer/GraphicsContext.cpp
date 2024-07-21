#include "TeaEngine/Renderer/GraphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tracy/Tracy.hpp>

namespace Tea {

    GraphicsContext::GraphicsContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
    {
		TEA_CORE_ASSERT(windowHandle, "Window handle is null!")
    }

    GraphicsContext::~GraphicsContext()
    {
    }

    void GraphicsContext::Init()
    {
        ZoneScoped;

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		TEA_CORE_ASSERT(status, "Failed to initialize Glad!");

        TEA_CORE_INFO("OpenGL Info:");
		TEA_CORE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		TEA_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		TEA_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		TEA_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Tea Engine requires at least OpenGL version 4.5!");
	}

    void GraphicsContext::SwapBuffers()
    {
        ZoneScoped;

        glfwSwapBuffers(m_WindowHandle);

        FrameMark;
    }

    Scope<GraphicsContext> GraphicsContext::Create(GLFWwindow* window)
    {
        return CreateScope<GraphicsContext>(window);
    }
}
