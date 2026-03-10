#include "CoffeeEngine/Renderer/GraphicsContext.h"
#include "CoffeeEngine/Core/Assert.h"

#include <glad/glad.h>
#include <tracy/Tracy.hpp>

namespace Coffee {

    GraphicsContext::GraphicsContext(SDL_Window* windowHandle) : m_WindowHandle(windowHandle)
    {
		COFFEE_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    GraphicsContext::~GraphicsContext()
    {
    }

    void GraphicsContext::Init()
    {
        ZoneScoped;

        m_Context = SDL_GL_CreateContext(m_WindowHandle);
        SDL_GL_MakeCurrent(m_WindowHandle, m_Context);
        int status = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
		COFFEE_CORE_ASSERT(status, "Failed to initialize Glad!");

        COFFEE_CORE_INFO("OpenGL Info:");
		COFFEE_CORE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		COFFEE_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		COFFEE_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		COFFEE_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Coffee Engine requires at least OpenGL version 4.5!");
	}

    void GraphicsContext::Shutdown()
    {
        ZoneScoped;

        SDL_GL_DestroyContext(m_Context);
    }

    void GraphicsContext::SwapBuffers()
    {
        ZoneScoped;

        SDL_GL_SwapWindow(m_WindowHandle);

        FrameMark;
    }

    bool GraphicsContext::SwapInterval(int interval)
    {
        ZoneScoped;

        return SDL_GL_SetSwapInterval(interval);
    }

    Scope<GraphicsContext> GraphicsContext::Create(SDL_Window* window)
    {
        return CreateScope<GraphicsContext>(window);
    }
}
